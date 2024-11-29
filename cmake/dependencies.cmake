include(FetchContent)

# Dependencies versions
set(BOX2D_VERSION "3.0.0")
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.15.0")
set(TMXLITE_VERSION "1.4.4")

# Define required libraries
set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
)

set(PROJECT_DEPENDENCIES
    ${SFML_LIBRARIES}
    tomlplusplus::tomlplusplus
    spdlog::spdlog
    box2d::box2d
    tmxlite::tmxlite
)

configure_sfml_build_options()
configure_box2d_build_options()

# Configure tmxlite build options
set(TMXLITE_STATIC_LIB TRUE CACHE BOOL "Build tmxlite as static lib")
set(TMXLITE_BUILD_EXAMPLES FALSE CACHE BOOL "Build tmxlite examples")
set(TMXLITE_BUILD_DOCS FALSE CACHE BOOL "Build tmxlite documentation")
set(TMXLITE_BUILD_TESTS FALSE CACHE BOOL "Build tmxlite tests")

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

# box2d
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG v${BOX2D_VERSION}
)

# tmxlite
FetchContent_Declare(
    tmxlite
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG master
)

FetchContent_GetProperties(tmxlite)
if(NOT tmxlite_POPULATED)
    FetchContent_Populate(tmxlite)
    add_subdirectory(${tmxlite_SOURCE_DIR}/tmxlite ${tmxlite_BINARY_DIR})
endif()

# Add tmxlite to project dependencies if not already present
if(NOT ";${PROJECT_DEPENDENCIES};" MATCHES ";tmxlite;")
    list(APPEND PROJECT_DEPENDENCIES tmxlite)
endif()

FetchContent_MakeAvailable(SFML tomlplusplus spdlog box2d tmxlite)
add_library(box2d::box2d ALIAS box2d)
add_library(tmxlite::tmxlite ALIAS tmxlite)
