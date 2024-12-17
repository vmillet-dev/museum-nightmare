cmake_minimum_required(VERSION 3.18)

if(OVERRIDED_VERSION)
    message("Version overrided by Github CI: ${OVERRIDED_VERSION}")

    if(OVERRIDED_VERSION MATCHES "^v(.*)")
        set(OVERRIDED_VERSION "${CMAKE_MATCH_1}")
    endif()
    set(PROJECT_VERSION ${OVERRIDED_VERSION})

else()
    set(PROJECT_VERSION "0.1.1")
    message("Project version: ${PROJECT_VERSION}")
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

# Copy needed folders or libs to avoid erros on pipeline or when running the exe on windows
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets/)
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls.cmake)
endif()
