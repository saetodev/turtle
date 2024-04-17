#include "turtle.h"

#include <GLFW/glfw3.h>

#include <lua/lauxlib.h>
#include <lua/lua.h>
#include <lua/lualib.h>

void set_renderer_size(u32 width, u32 height) {
    glMatrixMode(GL_PROJECTION);
    glOrtho(0.0, (f64)width, (f64)height, 0.0, 0.0, 1.0);
}

void draw_line(v2 start, v2 end, v4 color) {
    glBegin(GL_LINES);

    glColor4f(color.r, color.g, color.b, color.a);

    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);

    glEnd();
}

void draw_rectangle(v2 position, v2 size, v4 color) {
    glBegin(GL_QUADS);

    glColor4f(color.r, color.g, color.b, color.a);

    glVertex2f(position.x, position.y);
    glVertex2f(position.x + size.w, position.y);
    glVertex2f(position.x + size.w, position.y + size.h);
    glVertex2f(position.x, position.y + size.h);

    glEnd();
}

static int lua_key_down(lua_State *L) {
    u32 key = (u32)lua_tonumber(L, 1);

    if (key >= 0 || key < INPUT_BUF_SIZE) {
        bool val = state.input.keys[key].down;
        lua_pushboolean(L, val);
    } else {
        lua_pushboolean(L, false);
    }

    return 1;
}

static int lua_key_pressed(lua_State *L) {
    u32 key = (u32)lua_tonumber(L, 1);

    if (key >= 0 || key < INPUT_BUF_SIZE) {
        bool val = state.input.keys[key].pressed;
        lua_pushboolean(L, val);
    } else {
        lua_pushboolean(L, false);
    }

    return 1;
}

static int lua_key_released(lua_State *L) {
    u32 key = (u32)lua_tonumber(L, 1);

    if (key >= 0 || key < INPUT_BUF_SIZE) {
        bool val = state.input.keys[key].released;
        lua_pushboolean(L, val);
    } else {
        lua_pushboolean(L, false);
    }

    return 1;
}

static int lua_draw_line(lua_State *L) {
    v2 start = { 0 };
    v2 end = { 0 };
    v4 color = { 0 };
    
    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "r"); 
        color.r = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "g"); 
        color.g = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "b"); 
        color.b = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "a"); 
        color.a = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);

    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "x");
        end.x = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        end.y = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);

    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "x");
        start.x = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        start.y = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);

    draw_line(start, end, color);

    return 0;
}

static int lua_draw_rectangle(lua_State *L) {
    v2 position = { 0 };
    v2 size = { 0 };
    v4 color = { 0 };

    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "r"); 
        color.r = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "g"); 
        color.g = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "b"); 
        color.b = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
        
        lua_getfield(L, -1, "a"); 
        color.a = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);

    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "x");
        size.w = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        size.h = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);

    if (lua_istable(L, -1)) {
        lua_getfield(L, -1, "x");
        position.x = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);

        lua_getfield(L, -1, "y");
        position.y = (f32)lua_tonumber(L, -1);
        lua_pop(L, 1);
    }

    lua_pop(L, 1);

    draw_rectangle(position, size, color);

    return 0;
}

int main(void) {
    create_window(&state.window, 1280, 720);

    mem_arena_heap(&state.perm_memory, MEGABYTES(4));
    mem_arena_heap(&state.temp_memory, KILOBYTES(4));

    create_renderer(&state.renderer, RENDERER_MAX_QUADS, &state.perm_memory);
    set_renderer_size(state.window.width, state.window.height);

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    lua_register(L, "key_down", lua_key_down);
    lua_register(L, "key_pressed", lua_key_pressed);
    lua_register(L, "key_released", lua_key_released);

    lua_register(L, "draw_line", lua_draw_line);
    lua_register(L, "draw_rectangle", lua_draw_rectangle);

    luaL_dofile(L, "lua/script.lua"); 
   
    f32 speed = 75.0f;
    v2 position = make_vec2(32.0f, 32.0f);

    f32 last_time = 0.0f;

    while (!state.window.closed) {
        f32 now_time = (f32)glfwGetTime();
        f32 delta_time = now_time - last_time;
        last_time = now_time;

        glClear(GL_COLOR_BUFFER_BIT);

        lua_getglobal(L, "update");
        lua_pushnumber(L, delta_time);
        lua_call(L, 1, 0);

        update_window();    
    }

    return 0;
}
