cmake_minimum_required(VERSION 3.25)
project(Lab3a C)

set(CMAKE_C_STANDARD 99)

set(OP table/op/)
set(OP_SRC ${OP}insert.c ${OP}delete.c ${OP}search.c ${OP}import.c ${OP}print.c)
add_library(table STATIC table/table.c ${OP_SRC})

set(COMMON_SOURCES main.c menu.c utils.c)
add_executable(Lab3a ${COMMON_SOURCES})
target_link_libraries(Lab3a table)
