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

#define MAX_PAGES   10
typedef struct page_s {
    int ink;
    int paper;
    int ipage;
    SDL_Surface *surface;
} page_t;

static page_t _pages[MAX_PAGES];
static int _dpage, _wpage; /* display and write page index */
static int _num_pages;

static SDL_Window* _window;
static SDL_Surface *_wsurface=NULL;

/* Blit mode. 0=copy, 1=xor */
static uint8_t _blit = 0;

/* Remember width and height... */
static int _width, _height;

/* Utility functions. */
static uint32_t * _pixel_offset(int x, int y);
static bool _xor(uint8_t ifore, uint8_t iback, uint8_t *iresult);
static uint8_t _get_pixel(int x, int y);
static int _page_find(int page);

void gui_init(args_t *args) {
    
    /* Initialize the SDL. This is a critical operation. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        panic(CANT_INIT_GUI, "Can't initialize SDL: %s\n", SDL_GetError());

    /* If we are here, initialize the main window. */
    _width=args->width; _height=args->height;
    if ((_window = SDL_CreateWindow(
            args->title,
            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            _width,_height,
            SDL_WINDOW_SHOWN))==NULL)
        panic(CANT_INIT_GUI, "Can't create window: %s\n", SDL_GetError());

    /* Get window surface. */
    _wsurface = SDL_GetWindowSurface(_window);

    /* Initialize page 0 */
    _dpage=_wpage=0;
    _num_pages=1;
    _pages[0].ipage=0;
    _pages[0].ink=255;
    _pages[0].paper=0;


    /* Initialize 32 bit RGBA surface to use for blitting. */
    _pages[0].surface=SDL_CreateRGBSurface(
            0, _width, _height, 32,
            0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
}


void gui_exit() {
    for(int i=0;i<_num_pages;i++) SDL_FreeSurface(_pages[i].surface);
    SDL_DestroyWindow( _window );
    SDL_Quit();
}


void gui_cls() {
    /* Clears the surface. */
    SDL_FillRect(_pages[_wpage].surface, NULL, 
        SDL_MapRGBA(_pages[_wpage].surface->format, 0, _pages[_wpage].paper, 0, 0xff)
    );
}


void gui_run(bool(*process_commands)()) {

    /* Main loop. */
    bool quit = false;
    while( !quit )
    {     
        /* Copy work surface to screen surface. */
        SDL_BlitSurface(_pages[_dpage].surface, NULL, _wsurface, NULL );

        /* And show it... */
        SDL_UpdateWindowSurface(_window);

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
    _pages[_wpage].ink=intensity;
}

void gui_set_paper(uint8_t paper) {
    _pages[_wpage].paper=paper;
}

void gui_set_pixel(int x, int y) {

    if (_blit==BLIT_XOR) { /* XOR? */
        uint8_t iback = _get_pixel(x,y);
        uint8_t icurrent=_pages[_wpage].ink;
        if (_xor(icurrent,iback,&icurrent)) 
            *_pixel_offset(x,y)=SDL_MapRGBA(_pages[_wpage].surface->format, 0, icurrent, 0, 0xff);;
    } else
        *_pixel_offset(x,y)=SDL_MapRGBA(_pages[_wpage].surface->format, 0, _pages[_wpage].ink, 0, 0xff);

}


void gui_draw_line(int x0, int y0, int x1, int y1, uint8_t pattern) {

    uint8_t bit=0x80; /* Start with top bit... */
    
    int dx =  abs (x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs (y1 - y0), sy = y0 < y1 ? 1 : -1; 
    int err = dx + dy, e2; /* error value e_xy */

    for (;;){  /* Loop */
        
        /* Apply mask to foreground. */
        uint8_t ifore=pattern & bit ? _pages[_wpage].ink : 0;

        /* Blit mode? */
        if (_blit==BLIT_XOR) {
            /* Get current pixel at position... */
            uint8_t iback = _get_pixel(x0,y0);
            if (_xor(ifore,iback,&ifore)) 
                *_pixel_offset(x0,y0)=SDL_MapRGBA(_pages[_wpage].surface->format, 0, ifore, 0, 0xff);
        } else 
             *_pixel_offset(x0,y0)=SDL_MapRGBA(_pages[_wpage].surface->format, 0, ifore, 0, 0xff);
        /* Shift pattern. */
        bit>>=1;
        if (bit==0) bit=0x80;

        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
        if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
    }
}

void gui_set_blit(uint8_t blit) {
    _blit=blit;
}

void gui_set_wpage(int page) {
    int i=_page_find(page);
    if (i>=0) _wpage=i;
}

void gui_set_dpage(int page) {
    int i=_page_find(page);
    if (i>=0) _dpage=i;
}

/*****************************************************************************/


static int _page_find(int page) {
    int found=-1;
    for (int i=0;i<_num_pages && found==-1;i++) {
        if (_pages[i].ipage==page)
            found=i;
    }
    /* If page not found, create page... */
    if (found==-1) {
        /* Too many pages? */
        if (_num_pages==MAX_PAGES) return -1;
        /* Create new page... */
        found=_num_pages;
        _pages[_num_pages].ipage=page;
        _pages[_num_pages].surface=SDL_CreateRGBSurface(
                0, _width, _height, 32,
                0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
        _pages[_num_pages].ink=255;
        _pages[_num_pages].paper=0;
        /* Clear screen...*/
        SDL_FillRect(_pages[_num_pages].surface, NULL, 
            SDL_MapRGBA(_pages[_num_pages].surface->format, 0, _pages[_num_pages].paper, 0, 0xff)
        );
        _num_pages++;
    }
    return found;
}

static uint32_t * _pixel_offset(int x, int y) {
    return  
        (uint32_t *)_pages[_wpage].surface->pixels
        + y * _width /* pitch is fine, we have 32bpp and int32 */
        + x;
}

static uint8_t _get_pixel(int x, int y) {
    uint8_t *p =
        (uint8_t *)_pages[_wpage].surface->pixels
        + y * _pages[_wpage].surface->pitch
        + x * _pages[_wpage].surface->format->BytesPerPixel;
    return *(p+1); /* Get green component of pixel... */
}

static bool _xor(uint8_t ifore, uint8_t iback, uint8_t *iresult) {
    if (ifore==_pages[_wpage].ink && iback==_pages[_wpage].paper)
        return false; /* Don't change pixel. */
    else if (ifore==_pages[_wpage].paper && iback>_pages[_wpage].paper)
        return false;
    else if (ifore>_pages[_wpage].paper && iback==_pages[_wpage].paper) 
        *iresult=ifore;
    else
        *iresult=_pages[_wpage].paper;
    /* All false variants are exhausted. */
    return true;
}