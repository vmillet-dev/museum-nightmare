file(GLOB_RECURSE dll_files "${CMAKE_BINARY_DIR}/_deps/**/*.dll")

foreach(dll_file ${dll_files})
    file(COPY "${dll_file}" DESTINATION "${CMAKE_BINARY_DIR}")
    if(EXISTS "${CMAKE_BINARY_DIR}/Packaging")
        file(COPY "${dll_file}" DESTINATION "${CMAKE_BINARY_DIR}/Packaging/temp/")
    endif()
endforeach()