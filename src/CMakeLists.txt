cmake_minimum_required(VERSION 3.21)
project(program)

set(CMAKE_CXX_STANDARD 20)

add_executable(program main.cpp Loader.h Graph.h Vertex.h Random_assigner.h
            Smart_index.h Time_logger.h Smallest_Degree_Last.h Jones_Plassman.h)
