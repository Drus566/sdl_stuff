#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL3/SDL.h>

void input_init(void);
void input_poll(void);
bool input_quit_requested(void);
bool input_key_down(SDL_Keycode key);

#endif
