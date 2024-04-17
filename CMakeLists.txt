cmake_minimum_required(VERSION 3.20)

project(turtle C)
set(CMAKE_C_STANDARD 99)

file(GLOB SRC_FILES "src/*.c")

add_executable(turtle ${SRC_FILES})
target_link_libraries(turtle glfw GL lua m)
