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
    TTF_Font* font = TTF_OpenFont("KosugiMaru-Regular.ttf", 16);
    if (!font) {
        printf("Failed to load font: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        return -1;
    }

    const char* text = "申し訳ございませんがたくさんあります。";
    SDL_Color fg_color = {255, 255, 255, 255};
    SDL_Color bg_color = {0, 0, 0, 255};

    const char* ptr = text;

    // Surfaces
    // int text_length = getUTF8UnicodeCharCount(text);
    // printf("Array length, %d\n", array_length);
    SDL_Surface* surfaces[100] = {NULL};
    int surfaces_counter = 0;

    while (*ptr != '\0' && surfaces_counter < 100) {
      int char_len;
      uint32_t unicode_char = getUnicodeFromUTF8(ptr, &char_len);

      // Выводим информацию о символе
      printf("Symbol  %d: UTF-8: '%.*s' Unicode: U+%04X Length: %d byte\n", surfaces_counter, char_len, ptr, unicode_char, char_len);
      
      // Рендерим символ
      SDL_Surface* char_surface = TTF_RenderGlyph_LCD(font, unicode_char, fg_color, bg_color);
      if (!char_surface) {
        printf("Failed to create text surface for glyph U+%04X: %s\n", unicode_char, SDL_GetError());
        ptr += char_len;
        continue; // Skip if surface creation fails
      }

      surfaces[surfaces_counter++] = char_surface;

      ptr += char_len;
      // Перенос строки после каждых 10 символов
      // if (char_index % 10 == 0) {
      //   x = start_x;
      //   y += TTF_FontHeight(font) + 5;
      // }
    }

    
    // Calculate total width and max height for the combined surface
    int total_width = 0;
    int max_height = 0;
    SDL_Surface* surf = surfaces;
    while (surf != NULL) {
      total_width += surf->w + 5; // Add spacing between glyphs
      if (surf->h > max_height) {
        max_height = surf->h;
      }
      ++surf;
    }

    SDL_FRect rect = {5, 50, (float)total_width, (float)max_height};

    // Create the final combined surface
    SDL_Surface* combined_surface = SDL_CreateSurface(total_width, max_height, SDL_PIXELFORMAT_RGBA32);
    if (!combined_surface) {
      printf("Failed to create combined surface: %s\n", SDL_GetError());
      return -1;
    }

    // Blit each glyph onto the combined surface
    surf = surfaces;
    int x_offset = 0;
    while (surf != NULL) {
      SDL_Rect dest_rect = {x_offset, 0, surf->w, surf->h};
      SDL_BlitSurface(surf, NULL, combined_surface, &dest_rect);
      x_offset += surf->w + 5; // Move to the next position with spacing
      SDL_DestroySurface(surf); // Free individual surfaces after blitting
      surf = NULL;
    }    

    // Convert the combined surface to a texture
    SDL_Texture* combined_texture = SDL_CreateTextureFromSurface(renderer, combined_surface);
    SDL_DestroySurface(combined_surface); // Free the surface after conversion
    
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
      SDL_RenderTexture(renderer, combined_texture, NULL, &rect);
  
      SDL_RenderPresent(renderer);
    }
    
    SDL_DestroyTexture(combined_texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Shutdown SDL3 and TTF
    TTF_Quit();
    SDL_Quit();

    


    // Render each character as a surface
    // for (size_t i = 0; i < text.size(); i++) {
    //   SDL_Surface* textSurface = TTF_RenderGlyph_LCD(font, text[i], SDL_Color{255, 255, 255, 255}, SDL_Color{0, 0, 0, 255});
    //   if (!textSurface) {
    //       std::cerr << "Failed to create text surface: " << SDL_GetError() << std::endl;
    //       continue; // Skip if surface creation fails
    //   }
    //   surfaces.push_back(textSurface);
    // }
  


    // UTF-8 текст для SDL
    // char japanese_text[] = u8"申し訳ございませんがたくさんあります。";
    // size_t text_length_chars = utf8_strlen(japanese_text, 19);

    // printf("%zd\n", text_length_chars);

    // // Создаем массив для поверхностей
    // SDL_Surface** surfaces = malloc(text_length_chars * sizeof(SDL_Surface*));
    // if (!surfaces) {
    //     fprintf(stderr, "Ошибка выделения памяти\n");
    //     return 1;
    // }

    return 0;
}
