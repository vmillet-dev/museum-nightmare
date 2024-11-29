cmake_minimum_required(VERSION 3.16)

project(sfml_game VERSION 0.1.0 LANGUAGES CXX)

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

# Add source files
set(GAME_SOURCES
    src/game/Camera.cpp
    src/game/LevelLoader.cpp
    src/game/TextureManager.cpp
    src/game/objects/BackgroundObject.cpp
)

file(GLOB_RECURSE BASE_SOURCES
    "src/*.cpp"
)

# Remove main.cpp and GAME_SOURCES from BASE_SOURCES
list(FILTER BASE_SOURCES EXCLUDE REGEX ".*main\\.cpp$")
foreach(game_source ${GAME_SOURCES})
    list(FILTER BASE_SOURCES EXCLUDE REGEX "${game_source}$")
endforeach()

# Create executable
add_executable(${PROJECT_NAME}
    ${BASE_SOURCES}
    ${GAME_SOURCES}
    src/main.cpp
)

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/game
    ${CMAKE_SOURCE_DIR}/src/game/objects
    ${CMAKE_SOURCE_DIR}/src/game/levels
    ${CMAKE_BINARY_DIR}/_deps/tmxlite-src/include
    ${CMAKE_BINARY_DIR}/_deps/tmxlite-src/tmxlite/include
)

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    ${PROJECT_DEPENDENCIES}
    tmxlite
)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
