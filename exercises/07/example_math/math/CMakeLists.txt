add_library(math SHARED math.cpp math.hpp)

target_include_directories(math PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_compile_features(math PUBLIC cxx_std_17)
target_compile_options(math PRIVATE -Wall -Wextra -Wpedantic)

install(TARGETS math
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(FILES math.hpp DESTINATION include)
