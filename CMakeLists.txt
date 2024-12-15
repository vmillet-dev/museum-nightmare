cmake_minimum_required(VERSION 3.18)

project(sfml_game VERSION 0.1.0 LANGUAGES CXX)

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

# Link required libraries
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_DEPENDENCIES})

include(${CMAKE_SOURCE_DIR}/cmake/compiler_flags.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/package.cmake)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets/
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls.cmake)

    if(CMAKE_PACKAGE_ZIP)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory Packaging/temp
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets Packaging/temp/assets/
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls_for_packaging.cmake
            COMMAND ${CMAKE_COMMAND} -E copy 
                "$<TARGET_FILE:${PROJECT_NAME}>" 
                "${CMAKE_BINARY_DIR}/Packaging/temp/${PROJECT_NAME}$<TARGET_FILE_SUFFIX:${PROJECT_NAME}>"
            COMMAND powershell Compress-Archive
                -Path "${CMAKE_BINARY_DIR}/Packaging/temp/*"
                -DestinationPath "${CMAKE_BINARY_DIR}/Packaging/${PROJECT_NAME}.zip"
                -Force
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}/Packaging/temp"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()

