cmake_minimum_required(VERSION 3.15)

project(Ex07_2 VERSION 1.0
        DESCRIPTION "Exercise 2 from session 07"
        LANGUAGES CXX)

add_library(math STATIC math_lib.cpp math_lib.hpp)
target_compile_features(math PUBLIC cxx_std_17)

add_library(graphics STATIC graphics_lib.cpp graphics_lib.hpp)
target_link_libraries(graphics PRIVATE math)

add_executable(ex2 ex2.cpp)
target_link_libraries(ex2 PRIVATE graphics)
