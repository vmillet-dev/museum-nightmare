cmake_minimum_required(VERSION 3.16)

# Project version
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
set(PROJECT_VERSION_PATCH 0)

project(sfml_game VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Include utils and dependencies
include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Add source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Remove main.cpp from sources
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SFML_LIBRARIES}
    tomlplusplus::tomlplusplus
    spdlog::spdlog
    box2d::box2d
)

# Configure cross-platform compile options
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W4     # Warning level 4
            /WX     # Treat warnings as errors
            /MP     # Multi-processor compilation
            /EHsc   # Exception handling model
            /permissive-  # Strict C++ conformance
            /Zc:__cplusplus  # Enable correct __cplusplus macro
            /std:c++latest   # Use latest C++ features
        )
        # Disable specific warnings that might conflict with Box2D
        target_compile_definitions(${PROJECT_NAME} PRIVATE
            _CRT_SECURE_NO_WARNINGS
            NOMINMAX  # Prevent Windows.h from defining min/max macros
            WIN32_LEAN_AND_MEAN  # Exclude rarely-used Windows headers
        )
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wconversion
            -Wshadow
            -Wcast-align
            -Wunused
            -Wnull-dereference
            -fno-strict-aliasing  # Help with potential Box2D alignment issues
        )
    endif()
endif()

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
