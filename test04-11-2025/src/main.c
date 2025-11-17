#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create SDL Window
    SDL_Window* window = SDL_CreateWindow("SDL3 Unicode Texts", 420, 300, SDL_WINDOW_RESIZABLE);
    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return -1;
    }

    // Load Font with SDL3_tff
    TTF_Font* font = TTF_OpenFont("KosugiMaru-Regular.ttf", 16);
    if (!font) {
        std::cerr << "Failed to load font: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return -1;
    }

    // UTF-8 текст для SDL
    char japanese_text[] = u8"申し訳ございませんがたくさんあります。";
    size_t text_length_chars = utf8_strlen(japanese_text);


    // Создаем массив для поверхностей
    SDL_Surface** surfaces = malloc(text_length_chars * sizeof(SDL_Surface*));
    if (!surfaces) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }


    return 0;
}