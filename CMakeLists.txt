cmake_minimum_required(VERSION 3.20)

project(turtle C)
set(CMAKE_C_STANDARD 99)
set(CMAKE_BUILD_TYPE Debug)

file(GLOB SRC_FILES "src/*.c")

include_directories("glad/include" "cglm/include")

add_executable(turtle "glad/src/gl.c" ${SRC_FILES})
target_link_libraries(turtle glfw lua GL dl m)
