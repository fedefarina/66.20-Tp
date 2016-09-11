cmake_minimum_required(VERSION 3.5)
project(tp0)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")

set(SOURCE_FILES main.c calculo.c)
add_executable(tp0 ${SOURCE_FILES})