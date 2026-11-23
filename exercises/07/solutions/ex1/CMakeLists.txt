cmake_minimum_required(VERSION 3.15)

project(Ex07_1 VERSION 1.0
        DESCRIPTION "Exercise 1 from session 07"
        LANGUAGES CXX)

find_package(muparserx 4.0.12 EXACT CONFIG REQUIRED)
message(STATUS "muParserX library: ${muparserx_LIBRARIES}")

add_executable(ex1 ex1.cpp)
target_compile_features(ex1 PRIVATE cxx_std_17)
target_compile_options(ex1 PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(ex1 PRIVATE ${muparserx_INCLUDE_DIRS})
target_link_libraries(ex1 PRIVATE ${muparserx_LIBRARIES})
