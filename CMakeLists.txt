cmake_minimum_required(VERSION 3.16)

project(sfml_game VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows-specific configuration
if(MSVC)
    # Enable C11 atomics support for Box2D
    add_compile_definitions(_ENABLE_ATOMIC_ALIGNMENT_FIX)
    add_compile_options(/std:c11 /experimental:c11atomics)
endif()

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
    # Define OpenAL paths before target creation
    set(OPENAL_SDK_PATH "C:/Program Files (x86)/OpenAL 1.1 SDK" CACHE PATH "Path to OpenAL SDK")
    if(NOT EXISTS "${OPENAL_SDK_PATH}")
        message(WARNING "OpenAL SDK not found at default path. Please set OPENAL_SDK_PATH if OpenAL is installed elsewhere.")
        set(OPENAL_SDK_PATH $ENV{OPENAL_SDK_PATH} CACHE PATH "Path to OpenAL SDK" FORCE)
    endif()
endif()

# Add source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Remove main.cpp from sources
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)

# Configure OpenAL for Windows after target creation
if(WIN32 AND EXISTS "${OPENAL_SDK_PATH}")
    target_include_directories(${PROJECT_NAME} PRIVATE "${OPENAL_SDK_PATH}/include")

    # Copy OpenAL DLL to output directory
    if(EXISTS "${OPENAL_SDK_PATH}/libs/Win64/OpenAL32.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENAL_SDK_PATH}/libs/Win64/OpenAL32.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    else()
        message(WARNING "OpenAL32.dll not found at ${OPENAL_SDK_PATH}/libs/Win64/OpenAL32.dll")
    endif()
endif()

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SFML_LIBRARIES}
    ${PROJECT_DEPENDENCIES}
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
