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

option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Find OpenAL for Windows
if(WIN32)
    find_package(OpenAL)
    if(NOT OPENAL_FOUND)
        set(OPENAL_ROOT "$ENV{OPENAL_SDK_PATH}")
        if(NOT OPENAL_ROOT)
            set(OPENAL_ROOT "C:/Program Files (x86)/OpenAL 1.1 SDK")
        endif()

        if(EXISTS "${OPENAL_ROOT}")
            set(OPENAL_INCLUDE_DIR "${OPENAL_ROOT}/include")
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(OPENAL_LIBRARY "${OPENAL_ROOT}/libs/Win64/OpenAL32.lib")
                set(OPENAL_DLL "${OPENAL_ROOT}/redist/Win64/OpenAL32.dll")
            else()
                set(OPENAL_LIBRARY "${OPENAL_ROOT}/libs/Win32/OpenAL32.lib")
                set(OPENAL_DLL "${OPENAL_ROOT}/redist/Win32/OpenAL32.dll")
            endif()
        else()
            message(FATAL_ERROR "OpenAL SDK not found. Please install OpenAL SDK or set OPENAL_SDK_PATH.")
        endif()
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

# Include compiler flags after target creation
include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)

# Configure OpenAL for Windows after target creation
if(WIN32)
    if(OPENAL_FOUND)
        target_include_directories(${PROJECT_NAME} PRIVATE ${OPENAL_INCLUDE_DIR})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENAL_LIBRARY})

        if(EXISTS "${OPENAL_DLL}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${OPENAL_DLL}"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
                COMMENT "Copying OpenAL32.dll to output directory"
            )
        else()
            message(WARNING "OpenAL32.dll not found at ${OPENAL_DLL}")
        endif()
    endif()
endif()

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${SFML_LIBRARIES}
    ${PROJECT_DEPENDENCIES}
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
