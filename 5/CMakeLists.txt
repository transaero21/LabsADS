cmake_minimum_required(VERSION 3.25)
project(Lab5 C)

set(CMAKE_C_STANDARD 99)

add_library(graph STATIC graph/graph.c)
add_library(queue STATIC queue/queue.c)

add_executable(Lab5 main.c menu.c utils.c)
target_link_libraries(Lab5 graph queue)