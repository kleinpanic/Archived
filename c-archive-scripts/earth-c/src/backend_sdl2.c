#include "earth/backend.h"

#ifdef USE_SDL2

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    bool quit;
} sdl2_backend_data_t;

static void sdl2_init(backend_t *backend, config_t *config) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        exit(1);
    }

    sdl2_backend_data_t *data = malloc(sizeof(sdl2_backend_data_t));
    data->quit = false;

    data->window = SDL_CreateWindow(
        "Earth Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config->width,
        config->height,
        SDL_WINDOW_SHOWN
    );

    if (!data->window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        exit(1);
    }

    data->renderer = SDL_CreateRenderer(data->window, -1, SDL_RENDERER_ACCELERATED);
    if (!data->renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        exit(1);
    }

    data->texture = SDL_CreateTexture(
        data->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        config->width,
        config->height
    );

    if (!data->texture) {
        fprintf(stderr, "SDL_CreateTexture failed: %s\n", SDL_GetError());
        exit(1);
    }

    backend->user_data = data;
}

static void sdl2_present(backend_t *backend, framebuffer_t *fb) {
    sdl2_backend_data_t *data = (sdl2_backend_data_t *)backend->user_data;

    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT ||
            (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
            data->quit = true;
        }
    }

    // Update texture with framebuffer
    SDL_UpdateTexture(data->texture, NULL, fb->pixels, fb->stride * sizeof(uint32_t));

    // Render
    SDL_RenderClear(data->renderer);
    SDL_RenderCopy(data->renderer, data->texture, NULL, NULL);
    SDL_RenderPresent(data->renderer);
}

static bool sdl2_should_quit(backend_t *backend) {
    sdl2_backend_data_t *data = (sdl2_backend_data_t *)backend->user_data;
    return data->quit;
}

static void sdl2_shutdown(backend_t *backend) {
    sdl2_backend_data_t *data = (sdl2_backend_data_t *)backend->user_data;

    SDL_DestroyTexture(data->texture);
    SDL_DestroyRenderer(data->renderer);
    SDL_DestroyWindow(data->window);
    SDL_Quit();

    free(data);
}

backend_t *backend_sdl2_create(void) {
    backend_t *backend = malloc(sizeof(backend_t));
    backend->init = sdl2_init;
    backend->present = sdl2_present;
    backend->should_quit = sdl2_should_quit;
    backend->shutdown = sdl2_shutdown;
    backend->user_data = NULL;
    return backend;
}

#endif // USE_SDL2
