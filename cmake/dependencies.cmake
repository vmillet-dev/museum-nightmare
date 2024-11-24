include(FetchContent)

# Set version variables
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.12.0")

# Set fmt version
set(FMT_VERSION "10.1.1")

# Configure spdlog to use external fmt
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)

# Configure SFML build options
set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
set(SFML_GENERATE_PDB ON CACHE BOOL "" FORCE)  # Enable PDB for better debugging on Windows

# SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG ${SFML_VERSION}
)

# toml++
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v${TOMLPLUSPLUS_VERSION}
)

# fmt
FetchContent_Declare(
    fmt
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git
    GIT_TAG ${FMT_VERSION}
)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v${SPDLOG_VERSION}
)

# Make all dependencies available
FetchContent_MakeAvailable(SFML tomlplusplus fmt spdlog)
