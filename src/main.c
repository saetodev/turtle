#include "turtle.h"

#include <cglm/mat4.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

struct Buffer {
    size_t size;
    u8 *data;
};

static struct Buffer ReadEntireFile(const char *filename, struct Memory_Arena *memory) {
    FILE *file = fopen(filename, "rb");
    struct Buffer buffer = { 0 };

    if (!file) {
        //TODO: log this
        return buffer;
    }

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0, SEEK_SET);

    buffer.size = size;
    buffer.data = MemArenaAlloc(memory, size + 1);

    fread(buffer.data, sizeof(char), buffer.size, file);
    fclose(file);

    return buffer;
}

int main(void) {
    CreateWindow(&state.window, 1280, 720);

    MemArenaHeap(&state.perm_memory, MEGABYTES(4));
    MemArenaHeap(&state.temp_memory, KILOBYTES(4));

    CreateRenderer(&state.renderer, RENDERER_MAX_QUADS, &state.perm_memory);

    struct Buffer vertex_shader_src = ReadEntireFile("data/vertex_shader.glsl", &state.temp_memory);
    struct Buffer frag_shader_src = ReadEntireFile("data/fragment_shader.glsl", &state.temp_memory);

    u32 shader = CreateShader(vertex_shader_src.data, frag_shader_src.data); 
    glUseProgram(shader);

    mat4 projection_matrix;
    glm_mat4_identity(projection_matrix);
    glm_ortho(0.0f, (f32)state.window.width, (f32)state.window.height, 0.0f, 0.0f, 1.0f, projection_matrix);

    i32 location = glGetUniformLocation(shader, "u_projection"); 
    glUniformMatrix4fv(location, 1, GL_FALSE, &projection_matrix[0][0]);

    f32 last_time = 0.0f;

    vec2 position = { state.window.width / 2, state.window.height / 2 };
    f32 speed = 100.0f;

    while (!state.window.closed) {
        f32 now_time = (f32)glfwGetTime();
        f32 delta_time = now_time - last_time;
        last_time = now_time;

        {
            char *buf = MemArenaAlloc(&state.temp_memory, 256);
            sprintf(buf, "TURTLE | %f ms", delta_time * 1000.0f);
            glfwSetWindowTitle(state.window.ref, buf);
        }

        position[0] += (state.input.keys['D'].down - state.input.keys['A'].down) * speed * delta_time;
        position[1] += (state.input.keys['S'].down - state.input.keys['W'].down) * speed * delta_time;

        glClear(GL_COLOR_BUFFER_BIT);

        DrawRectangle(&state.renderer, position, (vec2){ 48.0f, 32.0f }, (vec4){ 0.5f, 0.5f, 0.85f, 1.0f });
        DrawRectangle(&state.renderer, (vec2){ 300.0f, 200.0f }, (vec2){ 250.0f, 100.0f }, (vec4){ 1.0f, 0.25f, 0.25f, 1.0f });
        
        PresentRenderer(&state.renderer);

        UpdateWindow();

        ClearMemArena(&state.temp_memory);
    }

    return 0;
}
