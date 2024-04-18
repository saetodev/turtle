#include "turtle.h"

#include <glad/gl.h>

//TODO: no error handling
u32 CreateShader(const char *vertex_shader_src, const char *frag_shader_src) {
    u32 program = glCreateProgram();

    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_src, NULL);
    glCompileShader(vertex_shader);

    u32 frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag_shader, 1, &frag_shader_src, NULL);
    glCompileShader(frag_shader);

    glAttachShader(program, vertex_shader);
    glAttachShader(program, frag_shader);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(frag_shader);
    glDeleteShader(vertex_shader);

    return program;
}

void SetShaderMat4(u32 shader, const char *name, mat4 m) {
    i32 location = glGetUniformLocation(shader, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

void CreateRenderer(struct Renderer *renderer, size_t max_quad_count, struct Memory_Arena *memory) {
    renderer->quad_count = 0;
    renderer->max_quad_count = max_quad_count;

    glCreateVertexArrays(1, &renderer->vao);
    glBindVertexArray(renderer->vao); 

    glCreateBuffers(1, &renderer->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const void *)offsetof(struct Vertex, position));
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(struct Vertex), (const void *)offsetof(struct Vertex, color));

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

void StartFrame(struct Renderer *renderer, u32 shader, struct Camera *camera) {
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shader);

    mat4 camera_view;
    GetCameraView(camera, camera_view);
    SetShaderMat4(shader, "u_mvp", camera_view);
}

void PresentRenderer(struct Renderer *renderer) { 
    size_t size = sizeof(struct Vertex) * renderer->quad_count * RENDERER_VERTICES_PER_QUAD;
    u32 count = renderer->quad_count * RENDERER_INDICES_PER_QUAD;

    glBindVertexArray(renderer->vao);
    glBindBuffer(GL_ARRAY_BUFFER, renderer->vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderer->ibo);

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

    vec3 real_pos = { position[0], position[1], 0.0f };
    vec3 real_size = { size[0], size[1], 1.0f };

    glm_translate(transform, position);
    glm_scale(transform, size);

    for (u32 i = 0; i < RENDERER_VERTICES_PER_QUAD; i++) {
        vec4 transformed_position;
        glm_mat4_mulv(transform, quad_vertex_position[i], transformed_position);

        renderer->vertex_buffer[offset + i].position[0] = transformed_position[0];
        renderer->vertex_buffer[offset + i].position[1] = transformed_position[1];

        renderer->vertex_buffer[offset + i].color[0] = color[0];
        renderer->vertex_buffer[offset + i].color[1] = color[1];
        renderer->vertex_buffer[offset + i].color[2] = color[2];
        renderer->vertex_buffer[offset + i].color[3] = color[3];
    }

    renderer->quad_count++;
}
