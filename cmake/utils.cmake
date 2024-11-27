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
endfunction()

# Configure Box2D build options
function(configure_box2d_build_options)
    # Basic Box2D options
    set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_STATIC ON CACHE BOOL "" FORCE)

    # Windows-specific Box2D configuration
    if(MSVC)
        # Set C++17 standard for Box2D
        set(CMAKE_CXX_STANDARD 17 CACHE STRING "" FORCE)
        set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL "" FORCE)
        set(CMAKE_CXX_EXTENSIONS OFF CACHE BOOL "" FORCE)

        # Use static runtime library
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "" FORCE)

        # Enable Box2D user settings
        set(BOX2D_USER_SETTINGS ON CACHE BOOL "" FORCE)

        # Add Windows-specific compile options
        add_compile_options(/W4 /WX- /EHsc /Zc:__cplusplus /std:c++17)
    endif()
endfunction()
