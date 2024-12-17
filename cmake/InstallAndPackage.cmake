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
install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets" DESTINATION ${ASSETS_INSTALL_DIR} COMPONENT assets)

foreach(DEP ${PROJECT_DEPENDENCIES})
    get_target_property(DEP_TYPE ${DEP} TYPE)
    message("dep type: ${DEP_TYPE}")
    install(FILES $<TARGET_FILE:${DEP}> DESTINATION ${LIBRARY_INSTALL_DIR} COMPONENT libs)
endforeach()

# CPack Configuration
if(WIN32)
    set(CPACK_GENERATOR "ZIP")
    set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
    set(CPACK_ARCHIVE_COMPONENT_INSTALL ON)
else()
    set(CPACK_GENERATOR "DEB")
    SET(CPACK_DEB_COMPONENT_INSTALL ON)
    set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Valentin MILLET <valentin.millet39@gmail.com>")
    set(CPACK_DEBIAN_PACKAGE_SECTION "games")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://your-project-homepage.com")

    # Debian package dependencies
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6")
endif()

SET(CPACK_COMPONENTS_ALL_IN_ONE_PACKAGE 1)
SET(CPACK_COMPONENTS_ALL app assets libs)

# Common CPack settings
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})

# Include CPack module
include(CPack)