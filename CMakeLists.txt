cmake_minimum_required(VERSION 3.10)
project(gmsh_membrane_mesh)

set(CMAKE_CXX_STANDARD 11)

include_directories(src)

add_executable(mesh
        src/main.cc
        src/parameters-utils.cc src/parameters-utils.h
        src/timing-utils.cc src/timing-utils.h
        )
