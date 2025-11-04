#include "render.h"
#include <SDL3/SDL.h>
#include <stdio.h>

static SDL_Window* g_window = NULL;
static SDL_Renderer* g_renderer = NULL;

bool render_init(const char* title, int width, int height) {
    g_window = SDL_CreateWindow(title, width, height, 0);
    if (!g_window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        return false;
    }
    g_renderer = SDL_CreateRenderer(g_window, NULL);
    if (!g_renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(g_window);
        g_window = NULL;
        return false;
    }
    return true;
}

void render_begin(void) {
    SDL_SetRenderDrawColor(g_renderer, 30, 30, 30, 255);
    SDL_RenderClear(g_renderer);
}

void render_end(void) {
    SDL_RenderPresent(g_renderer);
}

void render_shutdown(void) {
    if (g_renderer) SDL_DestroyRenderer(g_renderer);
    if (g_window) SDL_DestroyWindow(g_window);
    g_renderer = NULL;
    g_window = NULL;
}

void render_set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(g_renderer, r, g, b, a);
}

void render_fill_rect(const SDL_FRect* rect) {
    SDL_RenderFillRect(g_renderer, rect);
}

void render_get_window_size(int* w, int* h) {
    if (g_window) SDL_GetWindowSize(g_window, w, h);
    else { *w = 0; *h = 0; }
}
