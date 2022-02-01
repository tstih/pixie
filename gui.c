#include <stdbool.h>
#include <stdint.h>

#include <SDL2/SDL.h>

#include "panic.h"

static SDL_Window* _window;
static SDL_Renderer *_renderer;

/* Green component of line color. */
static uint8_t _intensity = 0xff;

void gui_init(int width, int height) {
    
    /* Initialize the SDL. This is a critical operation. */
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        panic(CANT_INIT_GUI, "Can't initialize SDL: %s\n", SDL_GetError());

    /* If we are here, initialize the main window. */
    if (SDL_CreateWindowAndRenderer(width,height,0,&_window,&_renderer)!=0)
        panic(CANT_INIT_GUI, "Can't create window: %s\n", SDL_GetError());
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

void gui_run() {

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
    }
}

void gui_set_intensity(uint8_t intensity) {
    _intensity=intensity;
    SDL_SetRenderDrawColor(_renderer, 0x00, _intensity, 0x00, 0x00);
}

void gui_set_pixel(int x, int y) {
    SDL_RenderDrawPoint(_renderer, 400, 300);
}