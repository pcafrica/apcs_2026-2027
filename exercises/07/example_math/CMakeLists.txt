cmake_minimum_required(VERSION 3.15)

project(MathExample VERSION 1.0
        DESCRIPTION "A small arithmetic library and application"
        LANGUAGES CXX)

include(CTest)

add_subdirectory(math)
add_subdirectory(apps)
