include_guard()

# Build the helper module
# Contains the path to the install etc.
configure_file(moduleHelper.cmake.in
               ${CMAKE_CURRENT_LIST_DIR}/moduleHelper.cmake @ONLY)

macro(add_module_test testName)
  add_test(NAME ModuleTest${testName}
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_LIST_DIR}/${testName}.cmake)
  # Need to install package before generating
  set_tests_properties(ModuleTest${testName} PROPERTIES FIXTURES_REQUIRED
                                     FixtureInstallPackage)
endmacro()

foreach(shouldSucceed simpleTolcTranslateFile)
  add_module_test(${shouldSucceed})
endforeach()

foreach(shouldFail noModuleName noLanguage noInputFile noOutput)
  add_module_test(${shouldFail})
  set_tests_properties(ModuleTest${shouldFail} PROPERTIES WILL_FAIL TRUE)
endforeach()
