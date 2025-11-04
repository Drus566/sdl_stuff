#include "engine.h"
#include "render.h"
#include "input.h"
#include "game.h"
#include <SDL3/SDL.h>
#include <stdint.h>
#include <stdio.h>

static bool running = false;

bool engine_init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS) != 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        return false;
    }
    if (!render_init(title, width, height)) {
        SDL_Quit();
        return false;
    }
    input_init();
    game_init();
    running = true;
    return true;
}

void engine_run(void) {
    const double target_dt = 1.0 / 60.0;
    uint64_t perf_freq = SDL_GetPerformanceFrequency();
    uint64_t last_counter = SDL_GetPerformanceCounter();
    while (running) {
        uint64_t now = SDL_GetPerformanceCounter();
        double dt = (double)(now - last_counter) / (double)perf_freq;
        last_counter = now;

        input_poll();
        if (input_quit_requested()) running = false;

        // Clamp dt to avoid spiral of death
        if (dt > 0.25) dt = 0.25;

        game_update((float)dt);

        render_begin();
        game_render();
        render_end();

        // simple sleep to yield CPU (not strictly necessary)
        SDL_Delay(1);
    }
}

void engine_shutdown(void) {
    game_shutdown();
    render_shutdown();
    SDL_Quit();
}
