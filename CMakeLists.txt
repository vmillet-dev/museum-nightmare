cmake_minimum_required(VERSION 3.18)

if(DEFINED ENV{GITHUB_REF} AND "$ENV{GITHUB_REF_TYPE}" STREQUAL "tag")
    string(REGEX REPLACE "^refs/tags/v?" "" PROJECT_VERSION "$ENV{GITHUB_REF}")
else()
    set(PROJECT_VERSION "0.1.0")
endif()

project(sfml_game VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set build type if not set
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

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

target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_DEPENDENCIES})

include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/InstallAndPackage.cmake)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets/)
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls.cmake)
endif()
