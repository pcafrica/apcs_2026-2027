cmake_minimum_required(VERSION 3.16)

project(linked_list LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(ex1 ex1.cpp list.cpp list.hpp node.hpp)
target_compile_options(ex1 PRIVATE -O0 -g -Wall -Wextra -Wpedantic)
