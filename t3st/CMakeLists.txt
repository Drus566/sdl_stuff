cmake_minimum_required(VERSION 3.15)
project(t3st C)

set(CMAKE_C_STANDARD 11)
add_executable(t3st
    src/main.c
    src/engine.c
    src/game.c
    src/render.c
    src/input.c
    src/memory.c
)

target_include_directories(t3st PRIVATE include)

find_package(SDL3 REQUIRED)
target_link_libraries(t3st PRIVATE SDL3::SDL3)
