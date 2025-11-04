#include "input.h"
#include <SDL3/SDL.h>
#include <stdbool.h>

static bool quit_requested = false;
static SDL_Event last_event;

void input_init(void) {
    quit_requested = false;
}

void input_poll(void) {
    while (SDL_PollEvent(&last_event)) {
        if (last_event.type == SDL_EVENT_QUIT) quit_requested = true;
        if (last_event.type == SDL_EVENT_KEY_DOWN) {
            if (last_event.key.key == SDLK_ESCAPE) quit_requested = true;
        }
    }
}

bool input_quit_requested(void) {
    return quit_requested;
}

bool input_key_down(SDL_Keycode key) {
    return (last_event.type == SDL_EVENT_KEY_DOWN && last_event.key.key == key);
}
