cmake_minimum_required(VERSION 3.24)
project(Lab3d C)

set(CMAKE_C_STANDARD 99)
set(COMMON_SOURCES main.c menu.c utils.c)

add_library(table STATIC table/table.c)

add_executable(Lab3d ${COMMON_SOURCES})
target_link_libraries(Lab3d table)
