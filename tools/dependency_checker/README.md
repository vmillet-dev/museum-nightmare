# Dependency Checker

C++ tool to check and update CMake dependency versions.

## Building
```bash
mkdir -p build && cd build
cmake ..
make
```

## Usage
```bash
./dependency_checker --dependencies-file path/to/dependencies.cmake
```

## Features
- Checks GitHub repositories for newer versions of dependencies
- Supports both direct version tags and version variables in CMake
- Updates dependencies.cmake file automatically when newer versions are found
- Maintains existing version prefix style (with or without 'v')

## Example
```bash
# Check dependencies in the main project
./dependency_checker --dependencies-file ../../cmake/dependencies.cmake
```

## Requirements
- CMake 3.18 or higher
- C++ compiler with C++17 support
- libcurl
- nlohmann_json
