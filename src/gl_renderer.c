#include "turtle.h"

#include <glad/gl.h>

void CreateRenderer(struct Renderer *renderer, size_t max_quad_count, struct Memory_Arena *memory) {
    renderer->quad_count = 0;
    renderer->max_quad_count = max_quad_count;

    glCreateVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao);

    glCreateBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    size_t vb_size = sizeof(struct Vertex) * max_quad_count * RENDERER_VERTICES_PER_QUAD;
    renderer->vertex_buffer = MemArenaAlloc(memory, vb_size);

    glBufferData(GL_ARRAY_BUFFER, vb_size, NULL, GL_DYNAMIC_DRAW);
    
    glCreateBuffers(1, &renderer->ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);

    size_t ib_size = sizeof(u32) * max_quad_count * RENDERER_INDICES_PER_QUAD;
    u32 *index_buffer = MemArenaAlloc(memory, ib_size);
    
    u32 offset = 0;
    for (u32 i = 0; i < (max_quad_count * RENDERER_INDICES_PER_QUAD); i += 6) {
        index_buffer[i + 0] = offset + 0;
        index_buffer[i + 1] = offset + 1;
        index_buffer[i + 2] = offset + 2;
    
        index_buffer[i + 3] = offset + 2;
        index_buffer[i + 4] = offset + 3;
        index_buffer[i + 5] = offset + 0;

        offset += 4;
    }

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, ib_size, index_buffer, GL_STATIC_DRAW);
    MemArenaPop(memory); 
}
