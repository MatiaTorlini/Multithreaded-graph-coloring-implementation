cmake_minimum_required(VERSION 3.21)
project(program)

set(CMAKE_CXX_STANDARD 20)

add_executable(program main.cpp Loader.h Graph.h Vertex.h Smallest_Degree_Last.h)
