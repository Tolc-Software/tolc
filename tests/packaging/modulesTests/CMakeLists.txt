include_guard()

# Build the helper module
configure_file(moduleHelper.cmake.in
               ${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake @ONLY)

foreach(shouldSucceed simpleTolcTranslateFile)
  add_test(NAME ModuleTest${shouldSucceed}
   COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/${shouldSucceed}.cmake)
  # Need to install package before generating
  set_tests_properties(ModuleTest${shouldSucceed} PROPERTIES FIXTURES_REQUIRED
                                     FixtureInstallPackage)
endforeach()

foreach(shouldFail noModuleName noLanguage noInputFile noOutput)
  add_test(NAME ModuleTest${shouldFail}
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/${shouldFail}.cmake)
  # Need to install package before generating
  set_tests_properties(ModuleTest${shouldFail} PROPERTIES FIXTURES_REQUIRED
                                     FixtureInstallPackage)
  set_tests_properties(ModuleTest${shouldFail} PROPERTIES WILL_FAIL TRUE)
endforeach()
