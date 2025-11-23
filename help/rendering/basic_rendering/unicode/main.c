#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// получение длинны utf8 символа
int getUTF8CharLength(const char* ptr) {
    unsigned char c = (unsigned char)*ptr;
    if ((c & 0x80) == 0) return 1;        // ASCII
    else if ((c & 0xE0) == 0xC0) return 2; // 2 байта
    else if ((c & 0xF0) == 0xE0) return 3; // 3 байта
    else if ((c & 0xF8) == 0xF0) return 4; // 4 байта
    return 1; // fallback
}

// Основная функция для подсчета Unicode символов
int getUTF8UnicodeCharCount(const char* utf8_str) {
  if (!utf8_str || *utf8_str == '\0') return 0;
  
  int count = 0;
  const char* ptr = utf8_str;
  
  while (*ptr != '\0') {
    int char_len = getUTF8CharLength(ptr);
    ptr += char_len;
    count++;
  }
  
  return count;
}

// Функция для извлечения Unicode кода из UTF-8 символа
uint32_t getUnicodeFromUTF8(const char* str, int* char_len) {
  *char_len = getUTF8CharLength(str);
  unsigned char* ustr = (unsigned char*)str;
  
  if (*char_len == 1) return ustr[0];
  else if (*char_len == 2) return ((ustr[0] & 0x1F) << 6) | (ustr[1] & 0x3F);
  else if (*char_len == 3) return ((ustr[0] & 0x0F) << 12) | ((ustr[1] & 0x3F) << 6) | (ustr[2] & 0x3F);
  else if (*char_len == 4) {
    return ((ustr[0] & 0x07) << 18) | ((ustr[1] & 0x3F) << 12) | 
            ((ustr[2] & 0x3F) << 6) | (ustr[3] & 0x3F);
  }
  return 0;
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
  TTF_Font* font = TTF_OpenFont("KosugiMaru-Regular.ttf", 24);
  if (!font) {
      printf("Failed to load font: %s\n", SDL_GetError());
      SDL_DestroyRenderer(renderer);
      SDL_DestroyWindow(window);
      return -1;
  }

  const char* text = "申し訳ございませんがたくさんあります。";
  SDL_Color fg_color = {255, 255, 255, 255};
  SDL_Color bg_color = {0, 0, 0, 255};

  int text_size = getUTF8UnicodeCharCount(text);
  SDL_Surface* text_surface = TTF_RenderText_LCD(font, text, text_size, fg_color, bg_color);

  SDL_FRect rect = {5, 50, (float)text_surface->w, (float)text_surface->h};
  
  // Convert the combined surface to a texture
  SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  SDL_DestroySurface(text_surface); // Free the surface after conversion

  // Event loop
  bool running = true;
  SDL_Event event;
  while (running) {
      while (SDL_PollEvent(&event)) {
          if (event.type == SDL_EVENT_QUIT || 
              (event.type == SDL_EVENT_KEY_DOWN && event.key.scancode == SDL_SCANCODE_ESCAPE)) {
              running = false;
          }
      }

      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
      SDL_RenderClear(renderer);

      // Render the combined text texture
      SDL_RenderTexture(renderer, texture, NULL, &rect);

      SDL_RenderPresent(renderer);
  }

  // Cleanup
  TTF_CloseFont(font);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);

  // Shutdown SDL3 and TTF
  TTF_Quit();
  SDL_Quit();
  
  return 0;
}
