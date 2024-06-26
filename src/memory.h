#ifndef TURTLE_MEMORY_H
#define TURTLE_MEMORY_H

#include "types.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define KILOBYTES(n) ((n) * 1024ll)
#define MEGABYTES(n) (KILOBYTES(n) * 1024ll)
#define GIGABYTES(n) (MEGABYTES(n) * 1024ll)

struct Memory_Arena {
    size_t  size;
    size_t  offset;
    size_t  last_offset;
    u8      *data;
};

static inline void panic(const char *message) {
    fprintf(stderr, "[PANIC]: %s\n", message);
    exit(EXIT_FAILURE);
}

static inline void *MemAlloc(size_t size) {
    void *mem = malloc(size);
    assert(mem);
    return mem;
}

static inline void *MemRealloc(void *ptr, size_t size) {
    void *mem = realloc(ptr, size);
    assert(mem);
    return mem;
}

static inline void MemArenaHeap(struct Memory_Arena *arena, size_t size) {
    assert(size > 0);

    arena->size = size;
    arena->offset = 0;
    arena->data = MemAlloc(size);
}

static inline void *MemArenaAlloc(struct Memory_Arena *arena, size_t size) {
    assert(size > 0);
    assert((arena->offset + size) <= arena->size);

    void *mem = arena->data + arena->offset;

    arena->last_offset = arena->offset;
    arena->offset += size;

    memset(mem, 0, size);

    return mem;
}

static inline void ClearMemArena(struct Memory_Arena *arena) {
    arena->offset = 0;
}

static inline void MemArenaPop(struct Memory_Arena *arena) {
    arena->offset = arena->last_offset;
}

#endif
