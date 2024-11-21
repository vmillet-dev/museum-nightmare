cmake_minimum_required(VERSION 3.16)
project(sfml_game)

# C++ Standard Configuration
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)
option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Common Libraries
set(COMMON_LIBS 
    sfml-graphics 
    sfml-window 
    sfml-system 
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)

# Source Files
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Common Object Library
add_library(common_objects OBJECT ${SOURCES})
target_link_libraries(common_objects PUBLIC
    ${COMMON_LIBS}
)

# Main Executable
add_executable(${PROJECT_NAME} $<TARGET_OBJECTS:common_objects> src/main.cpp)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    spdlog::spdlog
)

# Test Executable
add_executable(input_system_test $<TARGET_OBJECTS:common_objects> tests/input_system_test.cpp)
target_link_libraries(input_system_test PUBLIC
    ${COMMON_LIBS}
)

# Asset Copying (Single Command)
foreach(TARGET ${PROJECT_NAME} input_system_test)
    add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
endforeach()
