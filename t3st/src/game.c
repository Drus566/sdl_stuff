#include "game.h"
#include "render.h"
#include "input.h"
#include "memory.h"
#include <SDL3/SDL.h>
#include <stdlib.h>

static float x, y;
static float vx, vy;
static MemoryArena arena;
static uint8_t arena_buffer[1024 * 64];

void game_init(void) {
    arena_init(&arena, arena_buffer, sizeof(arena_buffer));
    x = 100.0f; y = 100.0f;
    vx = 120.0f; vy = 80.0f;
}

void game_update(float dt) {
    x += vx * dt;
    y += vy * dt;
    int w, h;
    render_get_window_size(&w, &h);
    if (x <= 0 || x >= w - 100) vx = -vx;
    if (y <= 0 || y >= h - 100) vy = -vy;

    if (input_key_down(SDLK_SPACE)) {
        // пример использования арены
        void* tmp = arena_alloc(&arena, 64);
        (void)tmp;
    }
}

void game_render(void) {
    SDL_FRect r = { x, y, 100, 100 };
    render_set_color(200, 30, 30, 255);
    render_fill_rect(&r);
    SDL_FRect inner = { x + 10, y + 10, 80, 80 };
    render_set_color(255, 220, 60, 255);
    render_fill_rect(&inner);
}

void game_shutdown(void) {
    arena_clear(&arena);
}
