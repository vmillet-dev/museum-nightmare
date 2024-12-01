cmake_minimum_required(VERSION 3.16)

project(sfml_game VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Disable compiler extensions

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# MSVC-specific settings
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /permissive- /Zc:preprocessor /Zc:__cplusplus")
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/utils.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Organize source files by component
set(CONFIG_SOURCES
    src/config/ConfigManager.cpp
    src/config/KeyMapper.cpp
)

set(INPUT_SOURCES
    src/input/devices/KeyboardDevice.cpp
    src/input/devices/MouseDevice.cpp
    src/input/devices/ControllerDevice.cpp
    src/input/InputManager.cpp
    src/input/providers/PlayerInputProvider.cpp
)

# Add all source files
file(GLOB_RECURSE OTHER_SOURCES
    "src/*.cpp"
)
list(FILTER OTHER_SOURCES EXCLUDE REGEX ".*main\\.cpp$")
list(FILTER OTHER_SOURCES EXCLUDE REGEX ".*(ConfigManager|KeyMapper|MouseMapper|KeyboardDevice|MouseDevice|ControllerDevice|InputManager|PlayerInputProvider)\\.cpp$")

# Create executable with organized sources
add_executable(${PROJECT_NAME}
    ${CONFIG_SOURCES}
    ${INPUT_SOURCES}
    ${OTHER_SOURCES}
    src/main.cpp
)

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_DEPENDENCIES})

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)
