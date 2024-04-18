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

void CreateCamera(struct Camera *camera, u32 width, u32 height) {
    camera->position[0] = 0.0f;
    camera->position[1] = 0.0f;

    glm_mat4_identity(camera->projection);
    //glm_ortho(0.0f, (f32)width, (f32)height, 0.0f, 0.0f, 1.0f, camera->projection);

    f32 half_width = (f32)width / 2.0f;
    f32 half_height = (f32)height / 2.0f;

    glm_ortho(-half_width, half_width, -half_height, half_height, 0.0f, 1.0f, camera->projection);
}

void GetCameraView(struct Camera *camera, mat4 dest) {
    vec3 real_position = { camera->position[0], camera->position[1] };

    mat4 transform;
    glm_mat4_identity(transform);
    glm_translate(transform, real_position);

    mat4 view;
    glm_mat4_inv(transform, view);

    glm_mat4_mul(camera->projection, view, dest);
}

int main(void) {
    CreateWindow(&state.window, 1280, 720);

    MemArenaHeap(&state.perm_memory, MEGABYTES(4));
    MemArenaHeap(&state.temp_memory, KILOBYTES(4));

    CreateRenderer(&state.renderer, RENDERER_MAX_QUADS, &state.perm_memory);

    struct Buffer vertex_shader_src = ReadEntireFile("data/vertex_shader.glsl", &state.temp_memory);
    struct Buffer frag_shader_src = ReadEntireFile("data/fragment_shader.glsl", &state.temp_memory);

    u32 shader = CreateShader(vertex_shader_src.data, frag_shader_src.data); 

    struct Camera camera = { 0 };
    CreateCamera(&camera, state.window.width, state.window.height);

    f32 last_time = 0.0f;

    vec2 position = { 32.0f, 32.0f };
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

        camera.position[0] += (state.input.keys['L'].down - state.input.keys['J'].down) * speed * delta_time;
        camera.position[1] += (state.input.keys['I'].down - state.input.keys['K'].down) * speed * delta_time;

        position[0] += (state.input.keys['D'].down - state.input.keys['A'].down) * speed * delta_time;
        position[1] += (state.input.keys['W'].down - state.input.keys['S'].down) * speed * delta_time;

        StartFrame(&state.renderer, shader, &camera);

        DrawRectangle(&state.renderer, (vec2){ 0.0f, 0.0f }, (vec2){ 250.0f, 100.0f }, (vec4){ 1.0f, 0.25f, 0.25f, 1.0f });
        DrawRectangle(&state.renderer, position, (vec2){ 32.0f, 32.0f }, (vec4){ 0.5f, 0.5f, 0.85f, 1.0f });
        
        PresentRenderer(&state.renderer);

        UpdateWindow();

        ClearMemArena(&state.temp_memory);
    }

    return 0;
}
