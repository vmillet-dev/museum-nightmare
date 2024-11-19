cmake_minimum_required(VERSION 3.16)
project(sfml_game)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include dependencies
include(${CMAKE_SOURCE_DIR}/cmake/Dependencies.cmake)

option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)

# Add source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Remove main.cpp from sources
list(FILTER SOURCES EXCLUDE REGEX ".*main\\.cpp$")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)

# Link SFML, toml++ and spdlog
target_link_libraries(${PROJECT_NAME} PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
    tomlplusplus::tomlplusplus
    spdlog::spdlog
)

# Copy resources to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources)
