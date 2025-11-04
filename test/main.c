#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Инициализация SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    // Создание окна
    SDL_Window* window = SDL_CreateWindow("SDL3 + CL Example", 800, 600, 0);
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Создание рендерера
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    printf("SDL3 успешно инициализирована!\n");
    printf("Управление:\n");
    printf("  ESC - выход\n");
    printf("  Пробел - сменить цвет\n");

    bool running = true;
    bool use_red_color = true;
    SDL_Event event;

    // Основной цикл
    while (running) {
        // Обработка событий
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;

                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        running = false;
                    }
                    if (event.key.key == SDLK_SPACE) {
                        use_red_color = !use_red_color;
                        printf("Цвет изменен: %s\n", use_red_color ? "Красный" : "Синий");
                    }
                    break;
            }
        }

        // Очистка экрана
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); // Темно-серый
        SDL_RenderClear(renderer);

        // Рисуем движущийся прямоугольник
        static float x = 100.0f, y = 100.0f;
        static float dx = 2.0f, dy = 1.5f;
        
        x += dx;
        y += dy;

        // Проверка границ
        int window_width, window_height;
        SDL_GetWindowSize(window, &window_width, &window_height);
        
        if (x <= 0 || x >= window_width - 100) dx = -dx;
        if (y <= 0 || y >= window_height - 100) dy = -dy;

        // Рисуем основной прямоугольник
        if (use_red_color) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Красный
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 100, 255, 255); // Синий
        }
        
        SDL_FRect rect = {x, y, 100, 100};
        SDL_RenderFillRect(renderer, &rect);

        // Рисуем внутренний прямоугольник
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Желтый
        SDL_FRect inner_rect = {x + 10, y + 10, 80, 80};
        SDL_RenderFillRect(renderer, &inner_rect);

        // Обновляем экран
        SDL_RenderPresent(renderer);

        // Небольшая задержка
        SDL_Delay(16);
    }

    // Очистка ресурсов
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Программа завершена.\n");
    return 0;
}