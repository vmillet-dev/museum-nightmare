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

    # Use C++17 for better atomic support
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # Set runtime library
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")

    # Add Windows-specific compile definitions
    add_compile_definitions(
        _ENABLE_ATOMIC_ALIGNMENT_FIX
        NOMINMAX  # Prevent Windows.h from defining min/max macros
        WIN32_LEAN_AND_MEAN  # Reduce Windows.h bloat
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

        # Use modern C++ for Box2D
        set_target_properties(box2d PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            MSVC_RUNTIME_LIBRARY ${CMAKE_MSVC_RUNTIME_LIBRARY}
            VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        )

        # Configure Box2D for Windows
        target_compile_definitions(box2d PRIVATE
            B2_USER_SETTINGS
            _ENABLE_ATOMIC_ALIGNMENT_FIX
            B2_HAS_ATOMIC=1
            B2_FORCE_INLINE=__forceinline
        )

        # Add threading support if needed
        if(NOT HAVE_ATOMIC_SUPPORT)
            find_package(Threads REQUIRED)
            target_link_libraries(box2d PRIVATE Threads::Threads)
        endif()

        message(STATUS "Box2D configuration complete")
    endif()
endif()

# OpenAL configuration for Windows
if(WIN32)
    set(OPENAL_ROOT "$ENV{OPENAL_SDK_PATH}" CACHE PATH "Path to OpenAL SDK")
    if(NOT OPENAL_ROOT)
        set(OPENAL_ROOT "C:/Program Files (x86)/OpenAL 1.1 SDK")
    endif()

    if(EXISTS "${OPENAL_ROOT}")
        message(STATUS "Found OpenAL SDK at: ${OPENAL_ROOT}")
        include_directories("${OPENAL_ROOT}/include")
        link_directories("${OPENAL_ROOT}/libs/Win64")
    else()
        message(FATAL_ERROR "OpenAL SDK not found. Please install it or set OPENAL_SDK_PATH environment variable.")
    endif()
endif()
