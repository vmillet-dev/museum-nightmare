include(FetchContent)

# Set version variables
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.12.0")

# SFML
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG ${SFML_VERSION}
)
FetchContent_MakeAvailable(SFML)

# toml++
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v${TOMLPLUSPLUS_VERSION}
)
FetchContent_MakeAvailable(tomlplusplus)

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v${SPDLOG_VERSION}
)
FetchContent_MakeAvailable(spdlog)
