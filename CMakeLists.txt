cmake_minimum_required(VERSION 3.16)
project(sfml_game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include dependencies
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Add source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Create an object library with common sources
add_library(common_objects OBJECT ${SOURCES})

# Main executable
add_executable(${PROJECT_NAME} 
    $<TARGET_OBJECTS:common_objects> 
    src/main.cpp
)

# Input system test executable
add_executable(input_system_test 
    $<TARGET_OBJECTS:common_objects> 
    tests/input_system_test.cpp
)

target_link_libraries(common_objects PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)

# Link SFML, toml++ and spdlog for main executable
target_link_libraries(${PROJECT_NAME} PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)

# Link libraries for input system test
target_link_libraries(input_system_test PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
add_custom_command(TARGET input_system_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
