if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets/
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/copy_dlls.cmake)
endif()

# Installation Directories
if(UNIX)
    set(EXECUTABLE_INSTALL_DIR "/usr/bin")
    set(ASSETS_INSTALL_DIR "/usr/share/${PROJECT_NAME}/assets")
    set(LIBRARY_INSTALL_DIR "/usr/lib")
else()
    set(EXECUTABLE_INSTALL_DIR ".")
    set(ASSETS_INSTALL_DIR ".")
    set(LIBRARY_INSTALL_DIR ".")
endif()

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${EXECUTABLE_INSTALL_DIR} COMPONENT app)
#install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION ${ASSETS_INSTALL_DIR})

#foreach(DEP ${PROJECT_DEPENDENCIES})
#    install(FILES $<TARGET_FILE:${DEP}> DESTINATION ${LIBRARY_INSTALL_DIR})
#endforeach()

# CPack Configuration
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
    set(CMAKE_INSTALL_DEFAULT_COMPONENT_NAME app)
    set(CPACK_COMPONENTS_ALL app)
else()
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Your Name <your.email@example.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://your-project-homepage.com")

    # Debian package dependencies
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
endif()

# Common CPack settings
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Your Project Description")
#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_PACKAGE_CONTACT "Your Name <your.email@example.com>")

# Include CPack module
include(CPack)