include_guard()

include(GNUInstallDirs)

# Install the tolc executable
install(TARGETS tolc DESTINATION ${CMAKE_INSTALL_BINDIR})
# The system includes should be next to the binary
install(DIRECTORY ${CMAKE_BINARY_DIR}/system_include
        DESTINATION ${CMAKE_INSTALL_BINDIR})
# Scripts that will be used by the support libraries
# Depends on them being next to the tolc executable
install(PROGRAMS ${PROJECT_SOURCE_DIR}/cmake/packaging/scripts/gather_headers.py
        DESTINATION ${CMAKE_INSTALL_BINDIR})

# Install the whole cmake/packaging/tolc directory to be where all the cmake modules lie
# (includeing tolcConfig.cmake)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/cmake/packaging/tolc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake)

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)
