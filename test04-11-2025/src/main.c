#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Инициализация чтобы было видно джойстик
    SDL_Init(SDL_HINT_JOYSTICK_THREAD);
    if (!SDL_HasGamepad()) {
        printf("No gamepads connected\n");
        SDL_Quit();
        return 0;
    }

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

    bool running = true;
    SDL_Event event;
    const int DEADZONE = 9000;


    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;
                
                case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
                    printf("Button pressed: %s\n", SDL_GetGamepadStringForButton((SDL_GamepadButton)event.gbutton.button));
                    break;

                case SDL_EVENT_GAMEPAD_BUTTON_UP:
                    printf("Button released: %s\n", SDL_GetGamepadStringForButton((SDL_GamepadButton)event.gbutton.button));
                    break;

                case SDL_EVENT_GAMEPAD_AXIS_MOTION:
                    printf("Axis moved: %s, Value: %d\n", SDL_GetGamepadStringForAxis((SDL_GamepadAxis)event.gaxis.axis), event.gaxis.value);

                    if(event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX && event.gaxis.value > DEADZONE) {
                        printf("left axis right\n");
                    } else if(event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTX && event.gaxis.value < -DEADZONE) {
                        printf("left axis left\n");
                    }

                    if(event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY && event.gaxis.value > DEADZONE) {
                                printf("left axis down\n");
                    } else if(event.gaxis.axis == SDL_GAMEPAD_AXIS_LEFTY && event.gaxis.value < -DEADZONE) {
                        printf("left axis up\n");
                    }
                    break;
            }
        }
        SDL_Delay(16); // Sleep 16ms (~60FPS)
    }

}