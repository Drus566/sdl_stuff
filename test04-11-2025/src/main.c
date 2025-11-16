#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
        SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC );

    // if (!SDL_HasGamepad()) {
    //     printf("No gamepads connected\n");
    //     SDL_Quit();
    //     return 0;
    // }
    printf("Gamepad found!\n");
    // Init window and renderer

    int count = 0;
SDL_JoystickID *ids = SDL_GetGamepads(&count);
SDL_Gamepad* gamepad = NULL;

// Iterate over the list of gamepads
for(int i = 0; i < count; i++) {
    SDL_Gamepad* gamepd = SDL_OpenGamepad(ids[i]);
    if(gamepad == NULL) {
        gamepad = gamepd;
    }
    
    printf("Gamepad connected: %s\n", SDL_GetGamepadName(gamepd));
    
    // Close the other gamepads
    if(i > 0) {
        SDL_CloseGamepad(gamepd);
    }
}
if (!gamepad) {
    printf("Failed to open gamepad: %s\n", SDL_GetError());
    SDL_Quit();
    return 1;
}

}