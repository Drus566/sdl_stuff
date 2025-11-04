#include "memory.h"
#include <string.h>

void arena_init(MemoryArena* a, void* backing, size_t size) {
    a->base = (uint8_t*)backing;
    a->size = size;
    a->used = 0;
}

void* arena_alloc(MemoryArena* a, size_t size) {
    size_t align = 8;
    size_t cur = (size_t)a->base + a->used;
    size_t mis = cur & (align - 1);
    size_t pad = (mis == 0) ? 0 : (align - mis);
    if (a->used + pad + size > a->size) return NULL;
    void* result = a->base + a->used + pad;
    a->used += pad + size;
    return result;
}

void arena_clear(MemoryArena* a) {
    a->used = 0;
    memset(a->base, 0, a->size);
}
