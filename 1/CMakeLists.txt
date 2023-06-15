cmake_minimum_required(VERSION 3.25)
project(Lab1 C)

set(CMAKE_C_STANDART 99)
set(COMMON_SOURCES main.c matrix.c utils.c)

add_executable(main ${COMMON_SOURCES})
