include_guard()

# See the README for documentation

# Create a fixture for installing tolc
set(test_package ${CMAKE_CURRENT_BINARY_DIR}/test_package/tolc)
add_test(
  NAME InstallPackage
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --install build --prefix ${test_package})
set_tests_properties(InstallPackage PROPERTIES FIXTURES_SETUP
                                               FixtureInstallPackage)

# Copy the package to the test consumer
add_test(NAME CopyPackage
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${test_package}
                 ${CMAKE_CURRENT_LIST_DIR}/consumer/build/tolc)
set_tests_properties(CopyPackage PROPERTIES FIXTURES_SETUP CopyDependencies)

# Need to generate package before copying it.
set_tests_properties(CopyPackage PROPERTIES FIXTURES_REQUIRED
                                            FixtureInstallPackage)

# Generate the consumer build files
add_test(
  NAME GenerateConsumer
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/consumer
  COMMAND
    ${CMAKE_COMMAND} -S. -Bbuild -GNinja
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_BUILD_TYPE=Debug)
set_tests_properties(GenerateConsumer PROPERTIES FIXTURES_SETUP
                                                 FixtureGenerateConsumer)

# Need to copy in the package before using it
set_tests_properties(GenerateConsumer PROPERTIES FIXTURES_REQUIRED
                                                 CopyDependencies)

# Build the consumer executable
add_test(
  NAME BuildConsumer
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/consumer
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_LIST_DIR}/consumer/build)
set_tests_properties(BuildConsumer PROPERTIES FIXTURES_SETUP
                                              FixtureBuildConsumer)
# Need to generate the build files before building
set_tests_properties(BuildConsumer PROPERTIES FIXTURES_REQUIRED
                                              FixtureGenerateConsumer)

add_test(NAME RunConsumer
         COMMAND ${CMAKE_CURRENT_LIST_DIR}/consumer/build/consumer)
# Need to build the consumer before running it
set_tests_properties(RunConsumer PROPERTIES FIXTURES_REQUIRED
                                            FixtureBuildConsumer)

add_test(NAME CleanUpConsumerBuildDirectory
         COMMAND ${CMAKE_COMMAND} -E rm -rf
                 ${CMAKE_CURRENT_LIST_DIR}/consumer/build)
# After running the final test, cleanup the build directory
set_tests_properties(CleanUpConsumerBuildDirectory PROPERTIES FIXTURES_CLEANUP
                                                              RunConsumer)
