#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // Init window and renderer
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer("SDL3 Keyboard", 800, 600, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/render: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    TTF_Font *font = TTF_OpenFont("FreeSans.ttf", 24);
    if (!font) {
        SDL_Log("Font load error: %s", SDL_GetError());
        return 1;
    }

    SDL_StartTextInput(window); // Ввод текста с клавиатуры

    bool running = true;

    SDL_Event event;

    // SDL_Scancode - ФИЗИЧЕСКОЕ расположение клавиши
    // SDL_Keycode - ЛОГИЧЕСКИЙ символ клавиши

    char text[1024] = "";
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                // timestamp - "временная метка" (в наносекундах)
                // windowID - "ID окна" (которое сейчас имеет фокус ввода)
                // which - "идентификатор клавиатуры" (полезно при нескольких клавиатурах)
                // scancode - "сканкод" (физическое расположение клавиши)
                // key - "символьный код" (логический символ в зависимости от раскладки)
                // mod - "модификаторы" (Shift, Ctrl, Alt и т.д.)
                // raw - "сырой сканкод" (низкоуровневый код конкретной платформы)
                // down - "состояние нажатия" (нажата/отпущена)
                // repeat - "флаг повтора" (сработал ли автоповтор при удержании)
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) running = false;
                    else if (event.key.key == SDLK_BACKSPACE) {
                        size_t len = strlen(text);
                        if (len > 0) text[len - 1] = '\0';
                    }
                    break;
                
                case SDL_EVENT_TEXT_INPUT:
                    if (strlen(text) + strlen(event.text.text) < sizeof(text) - 1) {
                        strcat(text, event.text.text);
                        printf("Текст: %s\n", text);
                    }
                    break;
                
                // SDL_EventType type; // SDL_EVENT_MOUSE_MOTION
                // Uint64 timestamp;   // In nanoseconds, populated using SDL_GetTicksNS()
                // SDL_WindowID windowID; // The window with mouse focus, if any
                // SDL_MouseID which;  // The mouse instance id in relative mode, SDL_TOUCH_MOUSEID for touch events, or 0
                // SDL_MouseButtonFlags state;       // The current button state
                // float x;            // X coordinate, relative to window
                // float y;            // Y coordinate, relative to window
                // float xrel;         // The relative motion in the X direction
                // float yrel;         // The relative motion in the Y direction    
                case SDL_EVENT_MOUSE_MOTION:
                    printf("Mouse moved to (%f, %f)\n", event.motion.x, event.motion.y);
                    break;
                

                // SDL_EventType type; // SDL_EVENT_MOUSE_BUTTON_DOWN or SDL_EVENT_MOUSE_BUTTON_UP
                // Uint64 timestamp;   // In nanoseconds, populated using SDL_GetTicksNS()
                // SDL_WindowID windowID; // The window with mouse focus, if any
                // SDL_MouseID which;  // The mouse instance id in relative mode, SDL_TOUCH_MOUSEID for touch events, or 0
                // Uint8 button;       // The mouse button index
                // bool down;          // true if the button is pressed
                // Uint8 clicks;       // 1 for single-click, 2 for double-click, etc.
                // float x;            // X coordinate, relative to window
                // float y;            // Y coordinate, relative to window
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    printf("Mouse button %d down at (%f, %f)\n", event.button.button, event.button.x, event.button.y);
                    break;

                case SDL_EVENT_MOUSE_BUTTON_UP:
                    printf("Mouse button %d up at (%f, %f)\n", event.button.button, event.button.x, event.button.y);
                    break;

                // SDL_EventType type; // SDL_EVENT_MOUSE_WHEEL
                // Uint64 timestamp;   // In nanoseconds, populated using SDL_GetTicksNS()
                // SDL_WindowID windowID; // The window with mouse focus, if any
                // SDL_MouseID which;  // The mouse instance id in relative mode or 0
                // float x;            // The amount scrolled horizontally, positive to the right and negative to the left
                // float y;            // The amount scrolled vertically, positive away from the user and negative toward the user
                // SDL_MouseWheelDirection direction; // Set to one of the SDL_MOUSEWHEEL_* defines. When FLIPPED the values in X and Y will be opposite. Multiply by -1 to change them back
                // float mouse_x;      // X coordinate, relative to window
                // float mouse_y;      // Y coordinate, relative to window
                // Sint32 integer_x;   // The amount scrolled horizontally, accumulated to whole scroll "ticks"
                // Sint32 integer_y;   // The amount scrolled vertically, accumulated to whole scroll "ticks"
                case SDL_EVENT_MOUSE_WHEEL:
                    printf("Mouse wheel: (%f, %f)\n", event.wheel.x, event.wheel.y);
                    break;
            }

            SDL_SetRenderDrawColor(renderer, 255,255,255,255); // Очистка экрана

            // Рендер текста
            // Создание текстуры текста
            SDL_Color black = {0, 0, 0};
            size_t len = strlen(text);
            SDL_Surface *surface = TTF_RenderText_Blended(font, text, len, black);
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

            // Рендер текстуры
            float texW = 0, texH = 0;
            SDL_GetTextureSize(texture, &texW, &texH);
            SDL_FRect dst = {50,50,texW,texH};

            SDL_RenderTexture(renderer, texture, NULL, &dst);

            SDL_DestroyTexture(texture);
            SDL_DestroySurface(surface);
        }
    }
}