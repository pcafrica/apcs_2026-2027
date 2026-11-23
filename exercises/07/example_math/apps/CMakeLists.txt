add_executable(math_app main.cpp)

target_link_libraries(math_app PRIVATE math)
target_compile_options(math_app PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS math_app RUNTIME DESTINATION bin)

if(BUILD_TESTING)
    add_test(NAME arithmetic COMMAND math_app)
endif()
