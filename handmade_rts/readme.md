handmade_rts/
├── build/                    # Папка сборки
├── src/
│   ├── os/                  # Платформо-зависимый код
│   │   ├── win32/
│   │   └── linux/
│   ├── game/               # Независимый от платформы код игры
│   │   ├── game.cpp
│   │   ├── renderer.cpp
│   │   ├── world.cpp
│   │   └── entities/
│   ├── libs/              # Внешние зависимости
│   │   └── sdl3/         # SDL3 как подмодуль git
│   └── data/             # Ресурсы игры
└── build.bat             # Простой скрипт сборки


// ВМЕСТО: множества разрозненных систем
typedef struct game_state {
    // ВСЕ данные игры в одной структуре
    world_state world;
    entity_system entities;
    render_system renderer;
    audio_system audio;
    ui_system ui;
} game_state;

// ВСЯ игра работает с этой одной структурой
void game_update_and_render(game_state* state, game_input* input) {
    // Все системы имеют доступ ко всем данным
}


// Structure of Arrays - ОПТИМАЛЬНО для обработки
typedef struct unit_system {
    // Позиции - обрабатываем вместе
    vec3* positions;        // 10000 * 12 bytes - смежные
    vec3* velocities;       // 10000 * 12 bytes - смежные  
    float* healths;         // 10000 * 4 bytes - смежные
    int* types;             // 10000 * 4 bytes - смежные
    
    // Состояния - обрабатываем вместе
    bool* selected;         // 10000 * 1 byte - смежные
    bool* moving;          // 10000 * 1 byte - смежные
    
    u32 count;
    u32 capacity;
} unit_system;

typedef struct rts_game_state {
    // ========== СИСТЕМА МИРА ==========
    chunked_grid world_grid;
    heightmap terrain;
    
    // ========== СИСТЕМА СУЩНОСТЕЙ ==========
    // UNIT SYSTEM - SOA подход
    u32 unit_count;
    u32 unit_capacity;
    
    // Трансформы - обрабатываем вместе
    vec3* unit_positions;
    vec3* unit_velocities;
    quat* unit_rotations;
    
    // Боевые характеристики - вместе
    float* unit_healths;
    float* unit_max_healths;
    int* unit_attack_powers;
    
    // Состояния - вместе
    u8* unit_states;        // MOVING, ATTACKING, IDLE, etc
    u8* unit_teams;
    u32* unit_target_ids;   // ID цели для атаки/движения
    
    // Визуальные компоненты - вместе
    u16* unit_model_ids;
    float* unit_anim_times;
    
    // ========== СИСТЕМА ВЫДЕЛЕНИЯ ==========
    u32 selected_unit_count;
    u32* selected_unit_indices; // Индексы выделенных юнитов
    
    // ========== СИСТЕМА ПУТЕЙ ==========
    path_request* path_requests;
    u32 path_request_count;
    
    // ========== СИСТЕМА РЕСУРСОВ ==========
    resource_nodes resource_nodes[256];
    u32 resource_node_count;
    
    // ========== ЭКОНОМИКА ==========
    float resources[RESOURCE_TYPE_COUNT];
    float resource_rates[RESOURCE_TYPE_COUNT];
    
    // ========== UI СИСТЕМА ==========
    ui_context ui;
    
} rts_game_state;