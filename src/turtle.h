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
    v2 position;
    v4 color;
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
void create_window(struct Window *window, u32 width, u32 height);

/*
 * Swaps opengl buffers and handles input events
 */
void update_window(void);

void create_renderer(struct Renderer *renderer, size_t max_quad_count, struct Memory_Arena *memory);

/*
 * Sets the size of the renderer's render target
 *
 * @param width the width of the renderer
 * @param height the height of the renderer
 */
void set_renderer_size(u32 width, u32 height);

/*
 *  Draws a line
 *
 *  @param start the x, y position of the start of the line
 *  @param end the x, y position of the end of the line
 *  @param color the color of the line
 */
void draw_line(v2 start, v2 end, v4 color);

/*
 *  Draws a rectangle
 *
 *  @param position the x, y position of the top left corner
 *  @param size the width and height of the rectangle
 *  @param color the color of the rectangle
 */
void draw_rectangle(v2 position, v2 size, v4 color);

#endif
