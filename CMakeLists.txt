cmake_minimum_required(VERSION 3.14)
project(marcomagic CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable warnings
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Create include directory for our magic_enum implementation
include_directories(include)

# Add subdirectories for different learning modules
add_subdirectory(examples)
add_subdirectory(step_by_step)

# Optional: Add benchmarks subdirectory if benchmark library is available
find_package(benchmark QUIET)
if(benchmark_FOUND)
    add_subdirectory(benchmarks)
endif()