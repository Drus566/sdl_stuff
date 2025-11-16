#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    if (!SDL_HasGamepad()) {
        printf("No gamepads connected\n");
        SDL_Quit();
        return 0;
    }
    printf("Gamepad found!\n");
    // Init window and renderer
    SDL_Init(SDL_INIT_VIDEO);
}