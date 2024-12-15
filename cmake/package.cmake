set(PACKAGING_DIR "Packaging")

# Create a function to create a temporary directory
function(CREATE_TEMP_DIR)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory ${PACKAGING_DIR}/temp
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()

# Create a function to copy assets
function(COPY_ASSETS)
  if(NOT ARGV)
    set(DESTINATION "${PACKAGING_DIR}/temp/assets")
  else()
    set(DESTINATION ${ARGV0})
  endif()
  message("Copy assets destination ${DESTINATION}")

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${DESTINATION}/assets/
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endfunction()

# Create a function to copy DLLs
function(COPY_DLLS)
  if(NOT ARGV)
    set(DESTINATION "${PACKAGING_DIR}/temp")
  else()
    set(DESTINATION ${ARGV0})
  endif()
  message("Copy dll destination ${DESTINATION}")

  file(GLOB_RECURSE DEP_DLLS LIST_DIRECTORIES false "${CMAKE_BINARY_DIR}/_deps/*-build/**/*.dll")
  foreach(DLL_FILE ${DEP_DLLS})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${DLL_FILE} ${DESTINATION}
      COMMENT "Copy dll ${DLL_FILE}"
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  endforeach()
endfunction()

# Create a function to copy executable
function(COPY_EXECUTABLE)
  file(GLOB_RECURSE executable LIST_DIRECTORIES false "${CMAKE_BINARY_DIR}/**/${PROJECT_NAME}.exe")
  foreach(exe ${executable})
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy ${exe} ${PACKAGING_DIR}/temp/
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
  endforeach()
endfunction()

# Create a function to create the zip archive
function(CREATE_WIN32_ZIP_ARCHIVE)
  add_custom_target(${PROJECT_NAME}_zip ALL
    COMMAND powershell Compress-Archive 
      -Path "${CMAKE_BINARY_DIR}/${PACKAGING_DIR}/temp/*" 
      -DestinationPath "${CMAKE_BINARY_DIR}/${PACKAGING_DIR}/${PROJECT_NAME}.zip" 
      -Force
    COMMAND ${CMAKE_COMMAND} -E rm -rf "${CMAKE_BINARY_DIR}/${PACKAGING_DIR}/temp"
    COMMENT "Creating zip archive for ${PROJECT_NAME}"
  )
  add_dependencies(${PROJECT_NAME}_zip ${PROJECT_NAME})
endfunction()

function(package_app)
    CREATE_TEMP_DIR()
    if (WIN32)
        COPY_ASSETS()
        COPY_DLLS()
        COPY_EXECUTABLE()
        CREATE_WIN32_ZIP_ARCHIVE()
    endif()
endfunction()