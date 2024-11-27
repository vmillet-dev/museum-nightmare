include(FetchContent)

# Dependencies versions
set(BOX2D_VERSION "3.0.0")  # Keep v3.0.0 but handle Windows properly
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.15.0")

# Define required libraries
set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
)

set(PROJECT_DEPENDENCIES
    tomlplusplus::tomlplusplus
    spdlog::spdlog
    box2d
)

# Windows-specific configuration
if(MSVC)
    # Enable C11 atomics support for MSVC
    add_compile_options(/std:c11 /experimental:c11atomics)
    add_compile_definitions(_ENABLE_ATOMIC_ALIGNMENT_FIX)
    # Set runtime library to match main project
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# Configure build options
configure_sfml_build_options()
configure_box2d_build_options()

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

# Make dependencies available
FetchContent_MakeAvailable(SFML tomlplusplus spdlog box2d)

# Post-fetch configuration for Box2D
if(TARGET box2d)
    if(MSVC)
        target_compile_options(box2d PRIVATE
            /std:c11
            /experimental:c11atomics
        )
        target_compile_definitions(box2d PRIVATE
            _ENABLE_ATOMIC_ALIGNMENT_FIX
            B2_USER_SETTINGS
        )
        set_target_properties(box2d PROPERTIES
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    endif()
endif()
