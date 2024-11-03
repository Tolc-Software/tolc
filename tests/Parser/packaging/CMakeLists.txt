include_guard()

# See the README for documentation

# Create a fixture for installing Parser
set(test_package ${CMAKE_CURRENT_BINARY_DIR}/test_package/parser)
add_test(
  NAME InstallPackage
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix ${test_package})
set_tests_properties(InstallPackage PROPERTIES FIXTURES_SETUP
                                               FixtureInstallPackage)

# Copy the package to the test consumer
add_test(NAME CopyPackage
         COMMAND ${CMAKE_COMMAND} -E copy_directory ${test_package}
                 ${CMAKE_CURRENT_LIST_DIR}/consumer/build/parser)
set_tests_properties(CopyPackage PROPERTIES FIXTURES_SETUP CopyDependencies)

# Need to generate package before copying it.
set_tests_properties(CopyPackage PROPERTIES FIXTURES_REQUIRED
                                            FixtureInstallPackage)

set(test_llvm_src ${CMAKE_BINARY_DIR}/_deps/llvm_entry-src)
if(EXISTS ${test_llvm_src})
  # Copy llvm into the consumer
  add_test(
    NAME CopyLLVM
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${test_llvm_src}
            ${CMAKE_CURRENT_LIST_DIR}/consumer/build/_deps/llvm_entry-src)
  set_tests_properties(CopyLLVM PROPERTIES FIXTURES_SETUP CopyDependencies)
endif()

# Generate the consumer build files
add_test(
  NAME GenerateConsumer
  WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/consumer
  COMMAND
    ${CMAKE_COMMAND} -S. -Bbuild -GNinja
    -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
    -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
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

add_test(NAME PackagingTest
         COMMAND ${CMAKE_CURRENT_LIST_DIR}/consumer/build/consumer)
# Need to build the consumer before running it
set_tests_properties(PackagingTest PROPERTIES FIXTURES_REQUIRED
                                              FixtureBuildConsumer)
