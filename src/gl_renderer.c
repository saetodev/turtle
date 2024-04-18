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

void PresentRenderer(struct Renderer *renderer) { 
    size_t size = sizeof(struct Vertex) * renderer->quad_count * RENDERER_VERTICES_PER_QUAD;
    u32 count = renderer->quad_count * RENDERER_INDICES_PER_QUAD;

    glBufferSubData(GL_ARRAY_BUFFER, 0, size, renderer->vertex_buffer);
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL);

    renderer->quad_count = 0;
}

void DrawRectangle(struct Renderer *renderer, vec2 position, vec2 size, vec4 color) {
    if (renderer->quad_count >= renderer->max_quad_count) {
        PresentRenderer(renderer);
    }

    u32 offset = renderer->quad_count * RENDERER_VERTICES_PER_QUAD;

    vec4 quad_vertex_position[] = {
        { -0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f, -0.5f, 0.0f, 1.0f },
        {  0.5f,  0.5f, 0.0f, 1.0f },
        { -0.5f,  0.5f, 0.0f, 1.0f },
    };

    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, position);
    glm_scale(transform, size);

    for (u32 i = 0; i < RENDERER_VERTICES_PER_QUAD; i++) {
        renderer->vertex_buffer[offset + i].position[0] = quad_vertex_position[i][0];
        renderer->vertex_buffer[offset + i].position[1] = quad_vertex_position[i][1];

        renderer->vertex_buffer[offset + i].color[0] = color[0];
        renderer->vertex_buffer[offset + i].color[1] = color[1];
        renderer->vertex_buffer[offset + i].color[2] = color[2];
        renderer->vertex_buffer[offset + i].color[3] = color[3];
    }

    renderer->quad_count++;
}
