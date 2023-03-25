project(ART
    LANGUAGES CXX)
cmake_minimum_required(VERSION 3.20)

set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR})

add_subdirectory(src/)

