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

static SDL_Window* _window;
static SDL_Renderer *_renderer;

/* Green component of line color. */
static uint8_t _intensity = 0xff;

/* Buffer for drawing lines pixel by pixel. */
#define MAX_POINTS 2048
static SDL_Point _pixels[MAX_POINTS];
static SDL_Point _spaces[MAX_POINTS];

void gui_init(args_t *args) {
    
    /* Initialize the SDL. This is a critical operation. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        panic(CANT_INIT_GUI, "Can't initialize SDL: %s\n", SDL_GetError());

    /* If we are here, initialize the main window. */
    if (SDL_CreateWindowAndRenderer(args->width,args->height,0,&_window,&_renderer)!=0)
        panic(CANT_INIT_GUI, "Can't create window: %s\n", SDL_GetError());

    SDL_SetWindowTitle(_window, args->title);
}

void gui_exit() {
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

void gui_set_pixel(int x, int y) {
    SDL_RenderDrawPoint(_renderer, x, y);
}

void gui_draw_line(int x0, int y0, int x1, int y1, uint8_t pattern) {
    if (pattern==0xff)
        SDL_RenderDrawLine(_renderer, x0, y0, x1, y1);
    else { /* We need to do Bresenham here. */
        
        uint8_t bit=0x80; /* Start with top bit... */
        int ipix=0,ispc=0;
        
        int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
        int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
        int err = dx + dy, e2; /* error value e_xy */
 
        for (;;){  /* Loop */
            
            /* Mask pixel... */
            if (pattern & bit) {
                _pixels[ipix].x=x0; _pixels[ipix].y=y0;
                ipix++;
            } else {
                _spaces[ispc].x=x0; _spaces[ispc].y=y0;
                ispc++;
            }
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
}