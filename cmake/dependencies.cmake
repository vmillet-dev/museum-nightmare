include(FetchContent)
include(CheckCXXSourceCompiles)

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

# Check for atomic support
check_cxx_source_compiles("
    #include <atomic>
    int main() {
        std::atomic<int> x;
        return 0;
    }"
    HAVE_ATOMIC_SUPPORT
)

if(NOT HAVE_ATOMIC_SUPPORT)
    find_package(Threads REQUIRED)
    link_libraries(${CMAKE_THREAD_LIBS_INIT})
endif()

# Windows-specific configuration
if(MSVC)
    message(STATUS "Configuring for MSVC build...")

    # Force C++17 for std::atomic support
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Add Windows-specific compile options
    add_compile_options(
        /W4     # Warning level 4
        /WX-    # Treat warnings as warnings (not errors)
        /MP     # Multi-processor compilation
        /EHsc   # Exception handling model
        /Zc:__cplusplus  # Enable proper __cplusplus macro
        /std:c++17       # Explicitly set C++17 mode
    )

    # Set runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

    # Add Box2D-specific definitions
    add_compile_definitions(
        B2_USER_SETTINGS
        B2_HAS_ATOMIC=1
        NOMINMAX
        WIN32_LEAN_AND_MEAN
    )
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
        message(STATUS "Configuring Box2D for MSVC...")

        # Configure Box2D properties
        set_target_properties(box2d PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            MSVC_RUNTIME_LIBRARY ${CMAKE_MSVC_RUNTIME_LIBRARY}
        )

        # Add Box2D-specific definitions
        target_compile_definitions(box2d PRIVATE
            B2_USER_SETTINGS
            B2_HAS_ATOMIC=1
            NOMINMAX
            WIN32_LEAN_AND_MEAN
        )

        message(STATUS "Box2D configuration complete")
    endif()
endif()

# OpenAL configuration for Windows
if(WIN32)
    # Try to find OpenAL SDK
    find_package(OpenAL QUIET)

    if(NOT OPENAL_FOUND)
        # Try multiple possible OpenAL SDK locations
        set(OPENAL_SEARCH_PATHS
            "$ENV{OPENAL_SDK_PATH}"
            "C:/Program Files (x86)/OpenAL 1.1 SDK"
            "C:/Program Files/OpenAL 1.1 SDK"
        )

        foreach(SEARCH_PATH ${OPENAL_SEARCH_PATHS})
            if(EXISTS "${SEARCH_PATH}")
                set(OPENAL_ROOT "${SEARCH_PATH}")
                break()
            endif()
        endforeach()

        if(OPENAL_ROOT)
            message(STATUS "Found OpenAL SDK at: ${OPENAL_ROOT}")

            # Add OpenAL include and library directories
            include_directories("${OPENAL_ROOT}/include")
            if(CMAKE_SIZEOF_VOID_P EQUAL 8)
                set(OPENAL_LIB_DIR "${OPENAL_ROOT}/libs/Win64")
            else()
                set(OPENAL_LIB_DIR "${OPENAL_ROOT}/libs/Win32")
            endif()
            link_directories("${OPENAL_LIB_DIR}")

            # Copy OpenAL32.dll to build directory during configuration
            if(EXISTS "${OPENAL_LIB_DIR}/OpenAL32.dll")
                file(COPY "${OPENAL_LIB_DIR}/OpenAL32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
                message(STATUS "Copied OpenAL32.dll to build directory")
            else()
                message(WARNING "OpenAL32.dll not found in ${OPENAL_LIB_DIR}")
            endif()
        else()
            message(STATUS "OpenAL SDK not found in standard locations. Using system OpenAL.")
        endif()
    endif()
endif()
