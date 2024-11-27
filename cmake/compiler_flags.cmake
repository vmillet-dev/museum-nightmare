# Configure cross-platform compile options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4     # Warning level 4
        /WX-    # Warnings as warnings, not errors
        /MP     # Multi-processor compilation
        /EHsc   # Exception handling model
        /Zc:__cplusplus  # Proper __cplusplus macro
        /std:c++17       # C++17 mode
    )

    # Add Windows-specific definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
    )
else()
    target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
        -Wcast-align
        -Wunused
        -Wnull-dereference
    )
endif()
