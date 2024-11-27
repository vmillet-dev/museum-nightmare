cmake_minimum_required(VERSION 3.16)

project(sfml_game VERSION 0.1.0)

# Set C++ standard before any targets are created
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Windows-specific configuration
if(MSVC)
    # Add Windows-specific compile options
    add_compile_options(
        /MP     # Enable multi-processor compilation
        /W4     # Warning level 4
        /wd4251 # Disable class needs to have dll-interface warning
        /wd4275 # Disable non dll-interface class warning
    )

    # Enable proper runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
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
    set(OPENAL_SDK_PATH "C:/Program Files (x86)/OpenAL 1.1 SDK" CACHE PATH "Path to OpenAL SDK")

    # Try environment variable if default path doesn't exist
    if(NOT EXISTS "${OPENAL_SDK_PATH}")
        set(OPENAL_SDK_PATH $ENV{OPENAL_SDK_PATH} CACHE PATH "Path to OpenAL SDK" FORCE)
    endif()

    if(NOT EXISTS "${OPENAL_SDK_PATH}")
        message(FATAL_ERROR "OpenAL SDK not found. Please install OpenAL SDK or set OPENAL_SDK_PATH.")
    endif()

    message(STATUS "Using OpenAL SDK at: ${OPENAL_SDK_PATH}")
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
    set(OPENAL_DLL "${OPENAL_SDK_PATH}/libs/Win64/OpenAL32.dll")
    if(EXISTS "${OPENAL_DLL}")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENAL_DLL}"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
            COMMENT "Copying OpenAL32.dll to output directory"
        )
    else()
        message(FATAL_ERROR "OpenAL32.dll not found at ${OPENAL_DLL}")
    endif()
endif()

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SFML_LIBRARIES}
    ${PROJECT_DEPENDENCIES}
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
