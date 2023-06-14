cmake_minimum_required(VERSION 3.24)
project(Lab3b C)

set(CMAKE_C_STANDARD 99)

set(OP table/op/)
set(OP_SRC ${OP}insert.c ${OP}delete.c ${OP}search.c ${OP}import.c ${OP}print.c)
add_library(table STATIC table/table.c ${OP_SRC})

set(COMMON_SOURCES main.c menu.c utils.c)
add_executable(Lab3b ${COMMON_SOURCES})
target_link_libraries(Lab3b table)
