include(FetchContent)

# Set version variables
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.15.0")

# Define required SFML libraries
set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
    sfml-audio
)

# Configure SFML build options before fetching
configure_sfml_build_options()

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

# spdlog
FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v${SPDLOG_VERSION}
)

# Make dependencies available
FetchContent_MakeAvailable(SFML tomlplusplus spdlog)
