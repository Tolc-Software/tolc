include_guard()

include(GNUInstallDirs)

# Install the tolc executable
install(TARGETS tolc DESTINATION ${CMAKE_INSTALL_BINDIR})
# The system includes should be next to the binary
install(DIRECTORY ${CMAKE_BINARY_DIR}/system_include
        DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)
