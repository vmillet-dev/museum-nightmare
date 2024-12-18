include(FetchContent)

# Dependencies versions
set(BOX2D_VERSION "3.0.0")
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.15.0")
set(TGUI_VERSION "1.6.1")

# Define required libraries
set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
)

set(PROJECT_DEPENDENCIES
    ${SFML_LIBRARIES}
    tomlplusplus::tomlplusplus
    spdlog
    box2d
    TGUI::TGUI
)

configure_sfml_build_options()
configure_box2d_build_options()
configure_tgui_build_options()

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

FetchContent_Declare(
    TGUI
    GIT_REPOSITORY https://github.com/texus/TGUI
    GIT_TAG v${TGUI_VERSION}
)

FetchContent_MakeAvailable(SFML tomlplusplus spdlog box2d TGUI)
