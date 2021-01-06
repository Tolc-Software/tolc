include_guard()

# See the README for documentation

foreach(relative_consumer_path consumer)
  set(consumer ${CMAKE_CURRENT_LIST_DIR}/${relative_consumer_path})
  # Create a fixture for creating a local install of tolc to test
  set(test_package ${CMAKE_CURRENT_BINARY_DIR}/test_package/tolc)
  add_test(
    NAME InstallPackage
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ${CMAKE_COMMAND} --install build --prefix ${test_package})
  set_tests_properties(InstallPackage PROPERTIES FIXTURES_SETUP
                                                 FixtureInstallPackage)

  # Generate the consumer build files
  # NOTE: Here we inject the path to the package that gets tested
  add_test(
    NAME GenerateConsumer
    WORKING_DIRECTORY ${consumer}
    COMMAND
      ${CMAKE_COMMAND} -S. -Bbuild -GNinja
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_BUILD_TYPE=Debug -Dtolc_ROOT=${test_package})
  set_tests_properties(GenerateConsumer PROPERTIES FIXTURES_SETUP
                                                   FixtureGenerateConsumer)

  # Build the consumer executable
  add_test(
    NAME BuildConsumer
    WORKING_DIRECTORY ${consumer}
    COMMAND ${CMAKE_COMMAND} --build ${consumer}/build)
  set_tests_properties(BuildConsumer PROPERTIES FIXTURES_SETUP
                                                FixtureBuildConsumer)
  # Need to generate the build files before building
  set_tests_properties(BuildConsumer PROPERTIES FIXTURES_REQUIRED
                                                FixtureGenerateConsumer)

  add_test(NAME RunConsumer
           COMMAND ${consumer}/build/consumer)
  # Need to build the consumer before running it
  set_tests_properties(RunConsumer PROPERTIES FIXTURES_REQUIRED
                                              FixtureBuildConsumer)

  add_test(NAME CleanUpConsumerBuildDirectory
           COMMAND ${CMAKE_COMMAND} -E rm -rf
                   ${consumer}/build)
  # After running the final test, cleanup the build directory
  set_tests_properties(CleanUpConsumerBuildDirectory PROPERTIES FIXTURES_CLEANUP
                                                                RunConsumer)
endforeach()
