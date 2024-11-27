cmake_minimum_required(VERSION 3.16)

project(sfml_game VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Find OpenAL for Windows
if(WIN32)
    set(OPENAL_SDK_PATH "C:/Program Files (x86)/OpenAL 1.1 SDK" CACHE PATH "Path to OpenAL SDK")
    if(NOT EXISTS "${OPENAL_SDK_PATH}")
        message(FATAL_ERROR "OpenAL SDK not found. Please set OPENAL_SDK_PATH to the correct location.")
    endif()

    # Add OpenAL include directory
    target_include_directories(${PROJECT_NAME} PRIVATE "${OPENAL_SDK_PATH}/include")

    # Copy OpenAL DLL to output directory
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${OPENAL_SDK_PATH}/libs/Win64/OpenAL32.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)
endif()

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
    ${PROJECT_DEPENDENCIES}
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
