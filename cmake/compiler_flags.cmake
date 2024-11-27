# Configure cross-platform compile options
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE
        /W4     # Warning level 4
        /WX     # Treat warnings as errors
        /MP     # Multi-processor compilation
        /EHsc   # Exception handling model
        /permissive-  # Strict C++ conformance
        /std:c11  # Enable C11 support
        /experimental:c11atomics  # Enable C11 atomics support
        /Zc:preprocessor  # Enable standards-conforming preprocessor
    )

    # Add atomic support definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        _ENABLE_ATOMIC_ALIGNMENT_FIX
    )

    # Copy OpenAL32.dll to output directory for Windows
    if(EXISTS "${OPENAL_SDK_PATH}/OpenAL32.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${OPENAL_SDK_PATH}/OpenAL32.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    endif()
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
