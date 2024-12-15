if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets/
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls.cmake)

    if(CMAKE_PACKAGE_ZIP)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory Packaging/temp
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets Packaging/temp/assets/
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls_for_packaging.cmake
            COMMAND ${CMAKE_COMMAND} -E copy 
                "$<TARGET_FILE:${PROJECT_NAME}>"  
                "${CMAKE_BINARY_DIR}/Packaging/temp/${PROJECT_NAME}$<TARGET_FILE_SUFFIX:${PROJECT_NAME}>"
            COMMAND powershell Compress-Archive
                -Path "${CMAKE_BINARY_DIR}/Packaging/temp/*"
                -DestinationPath "${CMAKE_BINARY_DIR}/Packaging/${PROJECT_NAME}.zip"
                -Force
            COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}/Packaging/temp"
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endif()