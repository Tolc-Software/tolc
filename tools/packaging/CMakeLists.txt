include_guard()

include(GNUInstallDirs)

# Install the tolc executable
install(TARGETS tolc DESTINATION ${CMAKE_INSTALL_BINDIR})
# The system includes should be next to the binary
install(DIRECTORY ${CMAKE_BINARY_DIR}/system_include
        DESTINATION ${CMAKE_INSTALL_BINDIR})

install(FILES ${PROJECT_SOURCE_DIR}/cmake/packaging/tolcConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tolc)

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)
