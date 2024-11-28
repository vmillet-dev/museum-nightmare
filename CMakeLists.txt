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
enable_testing()

# Add source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
)

# Remove main.cpp and test_parallax.cpp from sources
list(FILTER SOURCES EXCLUDE REGEX ".*(main|test_parallax)\\.cpp$")

# Create main executable
add_executable(${PROJECT_NAME} ${SOURCES} src/main.cpp)

# Create test executable
add_executable(test_parallax src/test_parallax.cpp ${SOURCES})

# Create unit tests executable
file(GLOB_RECURSE TEST_SOURCES
    "tests/*.cpp"
)
add_executable(unit_tests ${SOURCES} ${TEST_SOURCES})

# Add include directories for tests
target_include_directories(unit_tests PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Add include directories for main executables
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_include_directories(test_parallax PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_DEPENDENCIES})
target_link_libraries(test_parallax PRIVATE ${PROJECT_DEPENDENCIES})
target_link_libraries(unit_tests PRIVATE ${TEST_DEPENDENCIES})

# Add tests
add_test(NAME unit_tests COMMAND unit_tests)

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
add_custom_command(TARGET test_parallax POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
add_custom_command(TARGET unit_tests POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)
