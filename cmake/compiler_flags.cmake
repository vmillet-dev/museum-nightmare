# Configure cross-platform compile options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4     # Warning level 4
        /WX-    # Warnings as warnings, not errors
        /MP     # Multi-processor compilation
        /EHsc   # Exception handling model
        /Zc:__cplusplus  # Proper __cplusplus macro
        /std:c++17       # C++17 mode
        /std:c11         # C11 support
        /experimental:c11atomics  # Enable C11 atomics support
        /volatile:iso    # Proper volatile semantics
    )

    # Add Windows-specific definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        NOMINMAX
        WIN32_LEAN_AND_MEAN
        _CRT_SECURE_NO_WARNINGS
        B2_HAS_ATOMIC=1      # Enable Box2D atomic support
        B2_USER_SETTINGS=1   # Use custom Box2D settings
        _ENABLE_ATOMIC_ALIGNMENT_FIX # Fix atomic alignment issues
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
