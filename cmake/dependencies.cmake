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
        /WX-    # Warnings as warnings
        /MP     # Multi-processor compilation
        /EHsc   # Exception handling
        /Zc:__cplusplus  # Proper __cplusplus macro
        /std:c++17       # C++17 mode
    )

    # Set runtime library
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
FetchContent_MakeAvailable(SFML tomlplusplus spdlog)

# Configure Box2D separately
FetchContent_GetProperties(box2d)
if(NOT box2d_POPULATED)
    FetchContent_Populate(box2d)

    # Configure Box2D build options before adding subdirectory
    set(BOX2D_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_SHARED OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_STATIC ON CACHE BOOL "" FORCE)

    # Add Box2D subdirectory
    add_subdirectory(${box2d_SOURCE_DIR} ${box2d_BINARY_DIR})

    # Configure Box2D properties
    if(TARGET box2d)
        set_target_properties(box2d PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
        )

        # Add Box2D-specific definitions
        target_compile_definitions(box2d PRIVATE
            B2_USER_SETTINGS
            B2_HAS_ATOMIC=1
        )

        if(MSVC)
            set_target_properties(box2d PROPERTIES
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL"
            )

            target_compile_options(box2d PRIVATE
                /W4
                /WX-
                /EHsc
                /Zc:__cplusplus
                /std:c++17
            )

            target_compile_definitions(box2d PRIVATE
                NOMINMAX
                WIN32_LEAN_AND_MEAN
                _CRT_SECURE_NO_WARNINGS
            )
        endif()
    endif()
endif()

# Handle OpenAL for Windows
if(WIN32)
    set(OPENAL_ROOT "$ENV{OPENAL_SDK_PATH}" CACHE PATH "OpenAL SDK root directory")

    if(NOT OPENAL_ROOT)
        set(OPENAL_ROOT "C:/Program Files (x86)/OpenAL 1.1 SDK" CACHE PATH "OpenAL SDK root directory" FORCE)
    endif()

    if(EXISTS "${OPENAL_ROOT}")
        set(OPENAL_INCLUDE_DIR "${OPENAL_ROOT}/include")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(OPENAL_LIBRARY "${OPENAL_ROOT}/libs/Win64/OpenAL32.lib")
            set(OPENAL_BINARY "${OPENAL_ROOT}/redist/Win64/OpenAL32.dll")
        else()
            set(OPENAL_LIBRARY "${OPENAL_ROOT}/libs/Win32/OpenAL32.lib")
            set(OPENAL_BINARY "${OPENAL_ROOT}/redist/Win32/OpenAL32.dll")
        endif()

        if(EXISTS "${OPENAL_BINARY}")
            add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${OPENAL_BINARY}"
                $<TARGET_FILE_DIR:${PROJECT_NAME}>
            )
        endif()
    endif()
endif()
