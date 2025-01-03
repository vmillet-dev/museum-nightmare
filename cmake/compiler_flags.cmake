# Configure cross-platform compile options
if(CMAKE_BUILD_TYPE STREQUAL "Never")
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE
            /W4     # Warning level 4
            /WX     # Treat warnings as errors
            /MP     # Multi-processor compilation
            /EHsc   # Exception handling model
            /permissive-  # Strict C++ conformance
            /std:c11  # Enable C11 support
            /experimental:c11atomics  # Enable C11 atomics support
        )
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
            -Wconversion
            -Wshadow
            -Wcast-align
            -Wunused
            -Wnull-dereference
        )
    endif()
endif()
