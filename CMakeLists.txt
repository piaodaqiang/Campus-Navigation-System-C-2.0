cmake_minimum_required(VERSION 3.10)
project(campus_tour C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(campus_tour
        src/main.c
        src/graph.c
        src/util.c
)
