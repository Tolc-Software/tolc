include_guard()

# Build the helper module
configure_file(moduleHelper.cmake.in
               ${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake @ONLY)

foreach(moduleTest simpleTolcTranslateFile)
  add_test(NAME ModuleTest${moduleTest}
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/${moduleTest}.cmake)
  # Need to install package before generating
  set_tests_properties(ModuleTest${moduleTest} PROPERTIES FIXTURES_REQUIRED
                                                   FixtureInstallPackage)
endforeach()
