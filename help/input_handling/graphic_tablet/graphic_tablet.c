#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>

int main(int argc, char* argv[]) {
    // Init window and renderer
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!SDL_CreateWindowAndRenderer("SDL3 Pen Down", 800, 600, 0, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Render texture
    SDL_Texture *render_target = NULL;
    int w, h;

    // Получает размер области вывода рендерера (обычно - размер окна)
    SDL_GetRenderOutputSize(renderer, &w, &h);

    //     Альтернативы:
    // // Если у вас есть доступ к окну:
    // SDL_GetWindowSize(window, &w, &h);

    // // Для получения размера дисплея:
    // SDL_GetCurrentDisplayMode(0, &mode);
    // w = mode.w;
    // h = mode.h;

    // Создает текстуру для off-screen рендеринга
    render_target = SDL_CreateTexture(
        renderer,                    // рендерер
        SDL_PIXELFORMAT_RGBA8888,    // формат пикселей
        SDL_TEXTUREACCESS_TARGET,    // тип доступа - цель рендеринга
        w, h                         // размеры (из предыдущей строки)
    );

    // Популярные форматы
    // SDL_PIXELFORMAT_RGBA8888    // 8-8-8-8 ARGB (32-bit)
    // SDL_PIXELFORMAT_RGB24       // 8-8-8 RGB (24-bit)  
    // SDL_PIXELFORMAT_RGBA32      // 32-bit RGBA
    // SDL_PIXELFORMAT_BGRA32      // 32-bit BGRA (Windows)

    // Тип доступа
    // Ключевой параметр! Определяет как текстура будет использоваться:
    // SDL_TEXTUREACCESS_STATIC    // Не изменяется после создания
    // SDL_TEXTUREACCESS_STREAMING // Часто обновляется (lock/unlock)
    // SDL_TEXTUREACCESS_TARGET    // Может быть целью рендеринга ← наш случай

    if (!render_target) {
        SDL_Log("Couldn't create render target: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Default screen

    // Устанавливает текстуру как цель для рендеринга
    // Все последующие операции рисования будут идти НА эту текстуру
    SDL_SetRenderTarget(renderer, render_target);
    
    // Устанавливает цвет для операций рисования
    // Формат: R, G, B, A (Red, Green, Blue, Alpha)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    
    // Заливает ВСЮ текущую цель рендеринга установленным цветом
    // В нашем случае - заливает render_target белым цветом
    SDL_RenderClear(renderer);
    
    // Возвращает рендеринг обратно в основное окно
    // NULL = рендерим напрямую в окно
    //     ДО:                          ПОСЛЕ:
    // ┌─────────────────┐          ┌─────────────────┐
    // │   ТЕКСТУРА      │          │   ОКНО (экран)  │
    // │ [рендерим сюда] │ ───────▶ │ [рендерим сюда] │
    // └─────────────────┘          └─────────────────┘
    //               ┌─────────────────┐
    //               │   ТЕКСТУРА      │
    //               │ [готовый контент]│
    //               └─────────────────┘
    SDL_SetRenderTarget(renderer, NULL);
    // Включает альфа-блендинг (прозрачность) для операций рисования
    // SDL_BLENDMODE_NONE   // Без блендинга (полная непрозрачность)
    // SDL_BLENDMODE_BLEND  // Альфа-блендинг (учитывает прозрачность) ← наш случай
    // SDL_BLENDMODE_ADD    // Аддитивное смешение (для эффектов)
    // SDL_BLENDMODE_MOD    // Модуляция (умножение цветов)
    
    // Как работает блендинг
    // Исходный пиксель: (R1, G1, B1, A1)
    // Новый пиксель:    (R2, G2, B2, A2)
    // Результат: = исходный × (1 - A2) + новый × A2
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);


    // Info vars
    float pressure = 0.0f;
    
    float previous_touch_x = -1.0f;
    float previous_touch_y = -1.0f;

    float tilt_x = 0.0f;
    float tilt_y = 0.0f;

    float mouse_pressure = 1.0f;  // Фиктивное "давление" для мыши
    float mouse_x = -1.0f, mouse_y = -1.0f;

    bool running = true;

    // Event loop, process graphics

    while (running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    running = false;
                    break;

                case SDL_EVENT_PEN_DOWN:
                    printf("Pen down detected\n");
                    printf("Pen ID: %" SDL_PRIu32 "\n", event.ptouch.which);
                    printf("Window ID: %" SDL_PRIu32 "\n", event.ptouch.windowID);
                    printf("Position (%f, %f)\n", event.ptouch.x, event.ptouch.y);
                    break;
                
                // Движение пера (позиция X,Y)
                case SDL_EVENT_PEN_MOTION:
                    // Перо КАСАЕТСЯ экрана (рисуем)
                    if (pressure > 0.0f) {
                        // previous_touch_x = -1.0f означает "нет предыдущей точки"
                        if (previous_touch_x >= 0.0f) {
                            // Переключаем рендеринг НА текстуру:
                            SDL_SetRenderTarget(renderer, render_target);
                            // Устанавливаем цвет с ПЛАВАЮЩЕЙ ТОЧКОЙ и переменной прозрачностью
                            // R=0, G=0, B=0 → ЧЕРНЫЙ цвет
                            // A=pressure    → ПРОЗРАЧНОСТЬ зависит от силы нажатия
                            // Pressure: 0.1f → Почти прозрачная тонкая линия
                            // Pressure: 0.5f → Полужирная линия  
                            // Pressure: 1.0f → Полностью непрозрачная толстая линия
                            SDL_SetRenderDrawColorFloat(renderer, 0, 0, 0, pressure);
                            // Рисует линию от ПРЕДЫДУЩЕЙ точки к ТЕКУЩЕЙ
                            SDL_RenderLine(renderer, previous_touch_x, previous_touch_y, event.pmotion.x, event.pmotion.y);
                        }
                        // Сохраняем текущую позицию для следующего движения:
                        previous_touch_x = event.pmotion.x;
                        previous_touch_y = event.pmotion.y;
                    }
                    // Перо НАД экраном (не рисуем)
                    else {
                        previous_touch_x = previous_touch_y = -1.0f;
                    }
                    break;
                
                // Изменение параметров (давление, наклон и т.д.)
                // Когда возникает:
                // Изменилось давление на перо
                // Изменился наклон пера
                // Повернулось перо
                // Нажаты боковые кнопки
                case SDL_EVENT_PEN_AXIS:

                // Все возможные оси пера:
                // SDL_PEN_AXIS_PRESSURE    // Давление (0.0 - 1.0)
                // SDL_PEN_AXIS_XTILT       // Наклон по X (-1.0 - +1.0)  
                // SDL_PEN_AXIS_YTILT       // Наклон по Y (-1.0 - +1.0)
                // SDL_PEN_AXIS_DISTANCE    // Дистанция от поверхности
                // SDL_PEN_AXIS_ROTATION    // Вращение пера (0-360 градусов)
                // SDL_PEN_AXIS_SLIDER      // Ползунок на пере
                // SDL_PEN_AXIS_WHEEL       // Колесико на пере

                    // event.paxis.axis - какая ось изменилась
                    // event.paxis.value - значение оси (0.0 - 1.0)

                    // Обработка ДАВЛЕНИЯ
                    if (event.paxis.axis == SDL_PEN_AXIS_PRESSURE) {
                        pressure = event.paxis.value;
                        printf("Pressure %f\n", pressure);
                    }
                    // Наклон по X
                    else if (event.paxis.axis == SDL_PEN_AXIS_XTILT) {
                        tilt_x = event.paxis.value;
                    }
                    // Наклон по Y
                    else if (event.paxis.axis == SDL_PEN_AXIS_YTILT) {
                        tilt_y = event.paxis.value;
                    }
                    break;
                
                   // ⭐ ДОБАВЛЯЕМ поддержку МЫШИ
                    case SDL_EVENT_MOUSE_MOTION:
                    if (event.motion.state & SDL_BUTTON_LMASK) {  // Левая кнопка нажата
                        if (mouse_x >= 0.0f) {
                            SDL_SetRenderTarget(renderer, render_target);
                            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Фиксированная прозрачность
                            SDL_RenderLine(renderer, mouse_x, mouse_y, 
                                          event.motion.x, event.motion.y);
                        }
                        mouse_x = event.motion.x;
                        mouse_y = event.motion.y;
                    } else {
                        mouse_x = mouse_y = -1.0f;
                    }
                    break;

                    case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse_x = event.button.x;
                        mouse_y = event.button.y;
                        mouse_pressure = 1.0f;  // "Нажатие" для мыши
                    }
                    break;

                    case SDL_EVENT_MOUSE_BUTTON_UP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        mouse_x = mouse_y = -1.0f;
                        mouse_pressure = 0.0f;  // "Отпустили"
                    }
                    break;
            }

            // render
            char debug_text[1024];
            
            // Clear the screen
            // Переключаем рендеринг обратно в ОКНО (не в текстуру)
            SDL_SetRenderTarget(renderer, NULL);
            // Переключаем рендеринг обратно в ОКНО (не в текстуру)
            // R=255, G=255, B=255 → Белый
            // A=255 → Полностью непрозрачный
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
            // Заливаем ВСЁ окно белым цветом
            SDL_RenderClear(renderer);

            // Рендерим текстуру render_target на всё окно
            // render_target - текстура с нарисованным содержимым (рисунком)
            // NULL, NULL - отображать на всю область назначения (весь экран)
            //             ДО:                          ПОСЛЕ:
            // ┌─────────────────────────┐  ┌─────────────────────────┐
            // │        БЕЛЫЙ ФОН        │  │   ┌─────────────────┐   │
            // │                         │  │   │   РИСУНОК из    │   │
            // │                         │  │   │  render_target  │   │
            // │                         │  │   └─────────────────┘   │
            // └─────────────────────────┘  └─────────────────────────┘
            SDL_RenderTexture(renderer, render_target, NULL, NULL);

            // Альтернативы SDL_RenderTexture
            // Рендерим текстуру не на весь экран, а в определенной области
            // SDL_FRect dest_rect = {50, 50, 300, 300};  // x, y, w, h
            // SDL_RenderTexture(renderer, render_target, NULL, &dest_rect);

            // Устанавливаем ЧЁРНЫЙ цвет для текста
            // R=0, G=0, B=0 → Черный            
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            // Форматируем строку с значениями наклона
            // SDL_snprintf предотвращает переполнение буфера
            SDL_snprintf(debug_text, sizeof(debug_text), "Tilt: %f %f", tilt_x, tilt_y);
            // // Рендерит текст в указанной позиции
            SDL_RenderDebugText(renderer, 0, 8, debug_text);

            // Отображает всё отрендеренное на экране
            SDL_RenderPresent(renderer);

            //             ДО:                          ПОСЛЕ:
            // ┌─────────────────────────┐  ┌─────────────────────────┐
            // │ ВИДЕОПАМЯТЬ (невидимо)  │  │   ЭКРАН МОНИТОРА        │
            // │ • Белый фон             │  │ • Белый фон             │
            // │ • Рисунок из текстуры   │  │ • Рисунок из текстуры   │
            // │ • Отладочный текст      │  │ • Отладочный текст      │
            // └─────────────────────────┘  └─────────────────────────┘
        }
    }

    return 0;
}

// int main(int argc, char* argv[]) {
//     // Инициализация SDL
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         printf("SDL_Init failed: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Создание окна
//     SDL_Window* window = SDL_CreateWindow("SDL3 + CL Example", 800, 600, 0);
//     if (window == NULL) {
//         printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
//         SDL_Quit();
//         return 1;
//     }

//     SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
//     if  (renderer == NULL) {
//         printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
//         SDL_DestroyWindow(window);
//         SDL_Quit();
//         return 1;
//     }

//     printf("OK\n");
// }