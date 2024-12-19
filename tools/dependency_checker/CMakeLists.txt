cmake_minimum_required(VERSION 3.16)
project(dependency_checker VERSION 1.0.0)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

# Create the executable
add_executable(dependency_checker
    src/main.cpp
    src/github_api.cpp
    src/cmake_parser.cpp
)

target_link_libraries(dependency_checker PRIVATE
    CURL::libcurl
    nlohmann_json::nlohmann_json
)

install(TARGETS dependency_checker
    RUNTIME DESTINATION bin
)
