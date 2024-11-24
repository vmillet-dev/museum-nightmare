include(FetchContent)

# Configure SFML build options based on required libraries
function(configure_sfml_build_options)
    # Default all SFML modules to OFF
    set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_WINDOW OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_SYSTEM OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

    # Enable required modules based on SFML_LIBRARIES
    foreach(lib IN LISTS SFML_LIBRARIES)
        if(lib STREQUAL "sfml-graphics")
            set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
        elseif(lib STREQUAL "sfml-window")
            set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
        elseif(lib STREQUAL "sfml-system")
            set(SFML_BUILD_SYSTEM ON CACHE BOOL "" FORCE)
        elseif(lib STREQUAL "sfml-audio")
            set(SFML_BUILD_AUDIO ON CACHE BOOL "" FORCE)
        elseif(lib STREQUAL "sfml-network")
            set(SFML_BUILD_NETWORK ON CACHE BOOL "" FORCE)
        endif()
    endforeach()

    # Always disable these regardless of configuration
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(SFML_GENERATE_PDB ON CACHE BOOL "" FORCE)
endfunction()

# Set version variables
set(SFML_VERSION "2.6.2")
set(TOMLPLUSPLUS_VERSION "3.4.0")
set(SPDLOG_VERSION "1.15.0")

# Define required SFML libraries
set(SFML_LIBRARIES
    sfml-graphics
    sfml-window
    sfml-system
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
