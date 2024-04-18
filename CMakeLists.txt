cmake_minimum_required(VERSION 3.20)

project(turtle C)
set(CMAKE_C_STANDARD 99)

file(GLOB SRC_FILES "src/*.c")

include_directories("vendor/glfw/include" "vendor/glad/include" "vendor/cglm/include")
link_directories("vendor/glfw/lib-vc2022")

add_executable(turtle "vendor/glad/src/gl.c" ${SRC_FILES})
target_link_libraries(turtle glfw3 opengl32)
