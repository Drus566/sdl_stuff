#ifndef RENDER_H
#define RENDER_H

#include <SDL3/SDL.h>
#include <stdbool.h>

bool render_init(const char* title, int width, int height);
void render_begin(void);
void render_end(void);
void render_shutdown(void);
void render_set_color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
void render_fill_rect(const SDL_FRect* rect);
void render_get_window_size(int* w, int* h);

#endif
