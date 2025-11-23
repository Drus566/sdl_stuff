#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>

int main(void) {
  SDL_Init(SDL_INIT_VIDEO);

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

  SDL_Surface* healthbar_sur = IMG_Load("healthbar.png");
  if (!healthbar_sur) {
    printf("Failed to load image! IMG_Error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_Texture* healthbar_tex = SDL_CreateTextureFromSurface(renderer, healthbar_sur);
  SDL_DestroySurface(healthbar_sur); // Free surface after creating texture
  if (!healthbar_tex) {
    printf("Failed to create texture! IMG_Error: %s\n", SDL_GetError());
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 1;
  }

  SDL_FRect rect = {100, 100, 200, 22};

  int x = 0; // x position of the mouse

  bool running = true;
  
  while (running) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_EVENT_QUIT) {
        running = false;
      }
      if (e.type == SDL_EVENT_MOUSE_MOTION) {
        x = e.motion.x;
        if (x < 196) x = 196;
        if (x > 392) x = 392;
      }
    }

    SDL_FRect rect2 = {102.0f, 102.0f, (float)((x - 196) > 0 ? (x - 196) : 0), 18.0f};
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_RenderTexture(renderer, healthbar_tex, NULL, &rect);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &rect2);

    SDL_RenderPresent(renderer);

  }

  SDL_DestroyTexture(healthbar_tex);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}