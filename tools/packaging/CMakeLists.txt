include_guard()

include(GNUInstallDirs)

# Export the Frontend.py library
install(TARGETS tolc DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_BINARY_DIR}/system_include
        DESTINATION ${CMAKE_INSTALL_BINDIR})

set(CPACK_VERBATIM_VARIABLES YES)
# Allow packaging
include(CPack)
