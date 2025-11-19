#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

size_t utf8_strlen(char *str, size_t len) {
    size_t utf8_len = 0, idx = 0;
  
    for (unsigned char c = *str; idx < len; c = *(str + idx), utf8_len++) {
        size_t c_len;
        
        if ((c >> 7) == 0) {
          c_len = 1;
        } else if ((c >> 5) == 0b110) {
          c_len = 2;
        } else if ((c >> 4) == 0b1110) {
          c_len = 3;
        } else {
          c_len = 4;
        }
      
        idx += c_len;
    }
  
    return utf8_len;
}
  

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    // Create SDL Window
    SDL_Window* window = SDL_CreateWindow("SDL3 Unicode Texts", 420, 300, SDL_WINDOW_RESIZABLE);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return -1;
    }

    // Create Renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        return -1;
    }

    // Load Font with SDL3_tff
    TTF_Font* font = TTF_OpenFont("KosugiMaru-Regular.ttf", 16);
    if (!font) {
        printf("Failed to load font: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return -1;
    }

    // UTF-8 текст для SDL
    char japanese_text[] = u8"申し訳ございませんがたくさんあります。";
    size_t text_length_chars = utf8_strlen(japanese_text, 19);

    printf("%zd\n", text_length_chars);

    // Создаем массив для поверхностей
    SDL_Surface** surfaces = malloc(text_length_chars * sizeof(SDL_Surface*));
    if (!surfaces) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return 1;
    }

    return 0;
}
