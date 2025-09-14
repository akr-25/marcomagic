add_executable(basic_usage basic_usage.cpp)
target_include_directories(basic_usage PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_executable(advanced_usage advanced_usage.cpp)
target_include_directories(advanced_usage PRIVATE ${CMAKE_SOURCE_DIR}/include)