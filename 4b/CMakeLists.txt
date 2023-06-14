cmake_minimum_required(VERSION 3.25)
project(Lab4b C)

set(CMAKE_C_STANDARD 99)

set(OP tree/op/)
set(OP_SRC ${OP}print.c ${OP}traverse.c ${OP}insert.c ${OP}delete.c ${OP}import.c ${OP}search.c ${OP}diff.c)
add_library(tree STATIC tree/tree.c ${OP_SRC})

add_executable(Lab4b main.c menu.c utils.c)
target_link_libraries(Lab4b tree)
