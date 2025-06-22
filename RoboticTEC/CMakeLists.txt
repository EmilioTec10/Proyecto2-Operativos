cmake_minimum_required(VERSION 3.31)
project(RoboticTEC C)

set(CMAKE_C_STANDARD 11)

add_executable(RoboticTEC main.c
        client.c
        server.c
        nodo.c
        biblioteca.c
        biblioteca.h)
