include_guard()

# Create a fixture for creating a local install of tolc to test
set(test_package ${CMAKE_CURRENT_BINARY_DIR}/test_package/tolc)
add_test(
  NAME InstallPackage
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --install build --prefix ${test_package})
set_tests_properties(InstallPackage PROPERTIES FIXTURES_SETUP
                                               FixtureInstallPackage)

# See the README for more detailed documentation
foreach(system_test_name consumer)
  set(consumer ${CMAKE_CURRENT_LIST_DIR}/${system_test_name})

  # Generate the consumer build files
  # NOTE: Here we inject the path to the package that gets tested
  add_test(
    NAME Generate${system_test_name}
    WORKING_DIRECTORY ${consumer}
    COMMAND
      ${CMAKE_COMMAND} -S. -Bbuild -GNinja
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_BUILD_TYPE=Debug -Dtolc_ROOT=${test_package})
  set_tests_properties(Generate${system_test_name} PROPERTIES FIXTURES_SETUP
                                                   FixtureGenerate${system_test_name})
  # Need to install package before generating
  set_tests_properties(Generate${system_test_name} PROPERTIES FIXTURES_REQUIRED
                                                   FixtureInstallPackage)

  # Build the system test project
  add_test(
    NAME Build${system_test_name}
    WORKING_DIRECTORY ${consumer}
    COMMAND ${CMAKE_COMMAND} --build ${consumer}/build)
  set_tests_properties(Build${system_test_name} PROPERTIES FIXTURES_SETUP
                                                FixtureBuild${system_test_name})
  # Need to generate the build files before building
  set_tests_properties(Build${system_test_name} PROPERTIES FIXTURES_REQUIRED
                                                FixtureGenerate${system_test_name})

  # This requires that the project creates an executable called 'consumer' in the build directory
  add_test(NAME Run${system_test_name}
           COMMAND ${consumer}/build/consumer)
  # Need to build the consumer before running it
  set_tests_properties(Run${system_test_name} PROPERTIES FIXTURES_REQUIRED
                                              FixtureBuild${system_test_name})

  add_test(NAME CleanUp${system_test_name}BuildDirectory
           COMMAND ${CMAKE_COMMAND} -E rm -rf
                   ${consumer}/build)
  # After running the final test, cleanup the build directory
  set_tests_properties(CleanUp${system_test_name}BuildDirectory PROPERTIES FIXTURES_CLEANUP
                                                                Run${system_test_name})
endforeach()

# Simple tests
add_subdirectory(modulesTests)
