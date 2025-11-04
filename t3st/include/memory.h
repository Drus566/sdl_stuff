#ifndef MEMORY_H
#define MEMORY_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    uint8_t* base;
    size_t size;
    size_t used;
} MemoryArena;

void arena_init(MemoryArena* a, void* backing, size_t size);
void* arena_alloc(MemoryArena* a, size_t size);
void arena_clear(MemoryArena* a);

#endif
