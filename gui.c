/*
 * gui.c
 *
 * SDL bindings.
 *
 * MIT License (see: LICENSE)
 * copyright (c) 2022 tomaz stih
 *
 * 31.01.2022   tstih
 *
 */
#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "panic.h"
#include "args.h"
#include "exec.h"

#define BLIT_NONE   0
#define BLIT_COPY   1
#define BLIT_XOR    2

static SDL_Window* _window;
static SDL_Renderer *_renderer;
static SDL_Surface *_surface=NULL;

/* Green component of line color. */
static uint8_t _intensity = 0xff;

/* Blit mode. 0=copy, 1=xor */
static uint8_t _blit = 0;

/* Buffer for drawing lines pixel by pixel. */
#define MAX_POINTS 2048
static SDL_Point _pixels[MAX_POINTS];
static SDL_Point _spaces[MAX_POINTS];

/* Remember width and height... */
static int _width, _height;

void gui_init(args_t *args) {
    
    /* Initialize the SDL. This is a critical operation. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        panic(CANT_INIT_GUI, "Can't initialize SDL: %s\n", SDL_GetError());

    /* If we are here, initialize the main window. */
    _width=args->width; _height=args->height;
    if (SDL_CreateWindowAndRenderer(_width,_height,0,&_window,&_renderer)!=0)
        panic(CANT_INIT_GUI, "Can't create window: %s\n", SDL_GetError());

    SDL_SetWindowTitle(_window, args->title);

    /* Initialize 32 bit RGBA surface. */
    _surface=SDL_CreateRGBSurface(
            0, _width, _height, 32,
            0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
}

void gui_exit() {
    if (_surface!=NULL) SDL_FreeSurface(_surface);
    SDL_DestroyWindow( _window );
    SDL_Quit();
}

void gui_cls() {
    /* Clears the surface and restore the color. */
    SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(_renderer);
    SDL_SetRenderDrawColor(_renderer, 0x00, _intensity, 0x00, 0x00);
}

void gui_run(bool(*process_commands)()) {

    /* Clear screen first. */
    gui_cls();

    /* Main loop. */
    bool quit = false;
    while( !quit )
    {     
        /* Update the surface */
        SDL_RenderPresent(_renderer);

        /* Main loop. */
        SDL_Event e;
        while( SDL_PollEvent( &e ) != 0 )
            if( e.type == SDL_QUIT )
                quit = true;

        /* Execute commands. */
        quit &= process_commands();
    }
}

void gui_set_intensity(uint8_t intensity) {
    _intensity=intensity;
    SDL_SetRenderDrawColor(_renderer, 0x00, _intensity, 0x00, 0x00);
}

static uint8_t _gui_get_intentisty(int x, int y)
{
    /* Here p is the address to the pixel we want to retrieve */
    uint8_t *p = 
        (uint8_t *)_surface->pixels 
        + y * _surface->pitch 
        + x * _surface->format->BytesPerPixel;

    /* Intensity is green component (we only use ARGB 32, so this is
       guaranteed to be second byte. */
    return *(p+1);
}

static bool _xor(uint8_t ifore, uint8_t iback, uint8_t *iresult) {
    if (ifore==0 && iback==0)
        return false; /* Don't change pixel. */
    else if (ifore==0 && iback>0)
        return false;
    else if (ifore>0 && iback==0) 
        *iresult=ifore;
    else
        *iresult=0;
    /* All false variants are exhausted. */
    return true;
}

void gui_set_pixel(int x, int y) {

    if (_blit==BLIT_XOR) { /* XOR? */
        SDL_Rect rect = { x, y, 1, 1};
        SDL_RenderReadPixels(_renderer, &rect, SDL_PIXELFORMAT_ARGB8888, 
            _surface->pixels, _surface->pitch);
        uint8_t iback = ((uint8_t *)_surface->pixels)[1];
        uint8_t icurrent=_intensity, iresult;
        if (_xor(icurrent,iback,&iresult)) {
            gui_set_intensity(iresult);
            SDL_RenderDrawPoint(_renderer, x, y);
            gui_set_intensity(icurrent);
        }
    } else
        SDL_RenderDrawPoint(_renderer, x, y);

}

void gui_draw_line(int x0, int y0, int x1, int y1, uint8_t pattern) {

    
    if (_blit==BLIT_XOR) { /* XOR? */
        SDL_RenderReadPixels(_renderer, NULL, SDL_PIXELFORMAT_ARGB8888, 
            _surface->pixels, _surface->pitch);
    }

    uint8_t bit=0x80; /* Start with top bit... */
    int ipix=0,ispc=0;
    
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */

    for (;;){  /* Loop */
        
        /* Apply mask to foreground. */
        uint8_t ifore=pattern & bit ? _intensity : 0;

        /* Blit mode? */
        if (_blit==BLIT_XOR) {
            /* Get current pixel at position... */
            int iback=_gui_get_intentisty(x0,y0);
            if (_xor(ifore,iback,&ifore)) {
                if (ifore>0) { _pixels[ipix].x=x0; _pixels[ipix].y=y0; ipix++; }
                else { _spaces[ispc].x=x0; _spaces[ispc].y=y0; ispc++; }
            }
        } else {
            if (ifore > 0) { _pixels[ipix].x=x0; _pixels[ipix].y=y0; ipix++; } 
            else { _spaces[ispc].x=x0; _spaces[ispc].y=y0; ispc++; }
        }

        /* Shift pattern. */
        bit>>=1;
        if (bit==0) bit=0x80;

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }

    /* Now draw both pixel groups. */
    if (ispc > 0) {
        SDL_SetRenderDrawColor(_renderer, 0x00, 0x00, 0x00, 0x00);
        SDL_RenderDrawPoints(_renderer,_spaces,ispc);
    }
    if (ipix > 0) {
        SDL_SetRenderDrawColor(_renderer, 0x00, _intensity, 0x00, 0x00);
        SDL_RenderDrawPoints(_renderer,_pixels,ipix);
    }
}

void gui_set_blit(uint8_t blit) {
    _blit=blit;
}

