# Configure SFML build options based on required libraries
function(configure_sfml_build_options)
    # Disable all modules first
    set(SFML_BUILD_SYSTEM OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_WINDOW OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_GRAPHICS OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

    # Enable only required modules
    foreach(module IN LISTS SFML_LIBRARIES)
        if(module STREQUAL "sfml-system")
            set(SFML_BUILD_SYSTEM ON CACHE BOOL "" FORCE)
        elseif(module STREQUAL "sfml-window")
            set(SFML_BUILD_WINDOW ON CACHE BOOL "" FORCE)
        elseif(module STREQUAL "sfml-graphics")
            set(SFML_BUILD_GRAPHICS ON CACHE BOOL "" FORCE)
        endif()
    endforeach()

    # Disable unnecessary features
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(SFML_GENERATE_PDB ON CACHE BOOL "" FORCE)

    set(TOML_HEADER_ONLY OFF CACHE BOOL "" FORCE)
endfunction()

function(configure_box2d_build_options)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_SHARED ON CACHE BOOL "" FORCE)

    if(MSVC)
        # Ensure Box2D uses static runtime for MSVC
        set(BOX2D_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)
    endif()
endfunction()
