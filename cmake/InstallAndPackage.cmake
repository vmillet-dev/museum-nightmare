if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets/
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls.cmake)
endif()

# Installation Directories
if(UNIX)
    # Linux-specific installation paths
    set(EXECUTABLE_INSTALL_DIR "/usr/bin")
    set(ASSETS_INSTALL_DIR "/usr/share/${PROJECT_NAME}/assets")
    set(LIBRARY_INSTALL_DIR "/usr/lib")

    # Linux .so search in _deps/**/Release/
    file(GLOB THIRD_PARTY_LIBS
        "${CMAKE_BINARY_DIR}/_deps/**/Release/*.so"
    )
else()
    # Windows-specific installation paths
    set(EXECUTABLE_INSTALL_DIR ".")
    set(ASSETS_INSTALL_DIR ".")
    set(LIBRARY_INSTALL_DIR ".")

    file(GLOB THIRD_PARTY_LIBS
        "${CMAKE_BINARY_DIR}/_deps/**/Release/*.dll"
    )
endif()

# Install rules
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_DIR} COMPONENT applications)

if(EXISTS "${CMAKE_SOURCE_DIR}/assets")
    install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION ${ASSETS_INSTALL_DIR} COMPONENT assets)
endif()

# Install external libraries
if(THIRD_PARTY_LIBS)
    install(FILES ${THIRD_PARTY_LIBS} DESTINATION ${LIBRARY_INSTALL_DIR} COMPONENT libraries)
endif()

# CPack Configuration
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
    set(CPACK_COMPONENTS_ALL applications assets libraries)
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your.email@example.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://your-project-homepage.com")
    # Debian package dependencies
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
    # Determine package architecture
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
endif()

# Common CPack settings
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Your Project Description")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")

# Include CPack module
include(CPack)