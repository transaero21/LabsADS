cmake_minimum_required(VERSION 3.25)
project(Lab2 C)

set(CMAKE_C_STANDARD 99)
set(COMMON_SOURCES main.c list.c array.c queue.c passenger.c hub.c utils.c)

add_executable(aMain ${COMMON_SOURCES})

add_executable(lMain ${COMMON_SOURCES})
target_compile_options(lMain PUBLIC -DUSE_LIST)
