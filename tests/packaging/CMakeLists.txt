include_guard()

# NOTE: This fixture is used for all packaging tests
# Create a fixture for creating a local install of tolc to test
set(test_package ${CMAKE_CURRENT_BINARY_DIR}/test_package/tolc)
add_test(
  NAME InstallPackage
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --install build --prefix ${test_package})
set_tests_properties(InstallPackage PROPERTIES FIXTURES_SETUP
                                               FixtureInstallPackage)

add_subdirectory(systemTests)
add_subdirectory(moduleTests)
