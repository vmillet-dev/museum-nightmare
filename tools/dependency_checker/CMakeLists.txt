cmake_minimum_required(VERSION 3.18)
project(dependency_checker VERSION 1.0.0)

# Set C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Only include what we need
include(FetchContent)

# Define versions
set(NLOHMANN_JSON_VERSION "3.11.3")

# Find CURL package
find_package(CURL REQUIRED)

# Fetch JSON library
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v${NLOHMANN_JSON_VERSION}
)

FetchContent_MakeAvailable(nlohmann_json)

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
