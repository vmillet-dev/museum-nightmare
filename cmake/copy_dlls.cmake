file(GLOB_RECURSE DLL_FILES_1 "${CMAKE_BINARY_DIR}/_deps/**/${CMAKE_BUILD_TYPE}/*.dll")
file(GLOB_RECURSE DLL_FILES_2 "${CMAKE_BINARY_DIR}/_deps/**/**/${CMAKE_BUILD_TYPE}/*.dll")
file(GLOB_RECURSE DLL_FILES_3 "${CMAKE_BINARY_DIR}/_deps/**/extlibs/bin/x64/*.dll")
set(ALL_FILES ${DLL_FILES_1} ${DLL_FILES_2} ${DLL_FILES_3})

foreach(dll_file ${ALL_FILES})
    file(COPY "${dll_file}" DESTINATION "${CMAKE_BINARY_DIR}")
endforeach()