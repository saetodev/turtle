#ifndef TURTLE_H
#define TURTLE_H

#include "types.h"
#include "memory.h"

struct Window {
    u32     width;
    u32     height;
    bool    closed;

    void    *ref;
};

#define INPUT_BUF_SIZE 256

struct InputKey {
    bool down;
    bool pressed;
    bool released;
};

struct Input {
    struct InputKey keys[INPUT_BUF_SIZE];
};

#define RENDERER_MAX_QUADS          10000
#define RENDERER_VERTICES_PER_QUAD  4
#define RENDERER_INDICES_PER_QUAD   6

struct Vertex {
    vec2 position;
    vec4 color;
};

struct Renderer {
    u32             vao;
    u32             vbo;
    u32             ibo;

    size_t          quad_count;
    size_t          max_quad_count;

    struct Vertex   *vertex_buffer;
};

struct State {
    struct Window       window;
    struct Renderer     renderer;

    struct Memory_Arena perm_memory;
    struct Memory_Arena temp_memory;

    struct Input        input;
};

extern struct State state;

/*
 * Creates a new opengl context
 *
 * @param window the struct to initialize
 * @param width the width of the window
 * @param height the height of the window
 */
void CreateWindow(struct Window *window, u32 width, u32 height);

/*
 * Swaps opengl buffers and handles input events
 */
void UpdateWindow(void);

u32 CreateShader(const char *vertex_shader_src, const char *frag_shader_src); 

/*
 * Creates a new renderer
 *
 * @param renderer the struct to initialize
 * @param max_quad_count the maximum numbers of quads allowed in one draw call
 * @param memory the memory arena to allocate to
 */
void CreateRenderer(struct Renderer *renderer, size_t max_quad_count, struct Memory_Arena *memory);

/*
 * Pushes the current state of the renderer to the GPU
 *
 * @param renderer the rendering context
 */
void PresentRenderer(struct Renderer *renderer);

/*
 * Draws a rectangle
 *
 * @param renderer the rendering context
 * @param position the x,y position of the rectangle
 * @param size the size of the rectangle
 * @param color the color of the rectangle
 */
void DrawRectangle(struct Renderer *renderer, vec2 position, vec2 size, vec4 color);

#endif
