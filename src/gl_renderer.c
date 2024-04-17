#include "turtle.h"

void create_renderer(struct Renderer *renderer, size_t max_quad_count, struct Memory_Arena *memory) {
    
    renderer->quad_count = 0;
    renderer->max_quad_count = max_quad_count;

    renderer->vertex_buffer = mem_arena_alloc(memory, sizeof(struct Vertex) * max_quad_count * RENDERER_INDICES_PER_QUAD);

    u32 *index_buffer = mem_arena_alloc(memory, sizeof(u32) * max_quad_count * RENDERER_INDICES_PER_QUAD);
    mem_arena_pop(memory); 
}
