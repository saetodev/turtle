#include "turtle.h"

#include <GLFW/glfw3.h>

struct State state = { 0 };

static void window_close_callback(GLFWwindow *ref) {
    state.window.closed = true;
}

static void window_key_callback(GLFWwindow *ref, int key, int scancode, int action, int mods) {
    if (key < 0 || key >= INPUT_BUF_SIZE) {
        return;
    }

    bool is_down = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
    bool was_down = (action == GLFW_RELEASE) || (action == GLFW_REPEAT);

    state.input.keys[key].down = is_down;
    state.input.keys[key].pressed = is_down && !was_down;
    state.input.keys[key].released = !is_down && was_down;
}

void create_window(struct Window *window, u32 width, u32 height) {
    if (!glfwInit()) {
        panic("glfw init failed");
    }

    window->ref = glfwCreateWindow(width, height, "TURTLE", NULL, NULL);

    if (!window->ref) {
        panic("glfw create window failed");
    }

    glfwSetWindowCloseCallback(window->ref, window_close_callback);
    glfwSetKeyCallback(window->ref, window_key_callback);

    glfwMakeContextCurrent(window->ref);

    window->width = width;
    window->height = height;

    window->closed = false;
}

void update_window(void) {
    for (int i = 0; i < INPUT_BUF_SIZE; i++) {
        state.input.keys[i].pressed = false;
        state.input.keys[i].released = false;
    }

    glfwSwapBuffers(state.window.ref);
    glfwPollEvents();
}

