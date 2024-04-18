#include "turtle.h"

#include <GLFW/glfw3.h>

int main(void) {
    CreateWindow(&state.window, 1280, 720);

    MemArenaHeap(&state.perm_memory, MEGABYTES(4));
    MemArenaHeap(&state.temp_memory, KILOBYTES(4));

    CreateRenderer(&state.renderer, RENDERER_MAX_QUADS, &state.perm_memory);

    f32 last_time = 0.0f;

    while (!state.window.closed) {
        f32 now_time = (f32)glfwGetTime();
        f32 delta_time = now_time - last_time;
        last_time = now_time;

        
        UpdateWindow();    
    }

    return 0;
}
