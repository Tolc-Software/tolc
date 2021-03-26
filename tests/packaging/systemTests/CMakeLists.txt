include_guard()

# See the README for more detailed documentation

# On Windows CMAKE_C_COMPILER is not set by default
if(MSVC)
  set(c_compiler clang-cl)
  set(cxx_compiler clang-cl)
else()
  set(c_compiler ${CMAKE_C_COMPILER})
  set(cxx_compiler ${CMAKE_CXX_COMPILER})
endif()

# Generate helpers for the tests
foreach(systemTestName translateFileTest translateLibraryTest addLibraryTest createTranslationTest)
  # Path to the consumer project
  set(consumer ${CMAKE_CURRENT_LIST_DIR}/${systemTestName})

  # Generate the consumer build files
  # NOTE: Here we inject the path to the package that gets tested
  add_test(
    NAME Generate${systemTestName}
    WORKING_DIRECTORY ${consumer}
    COMMAND
      ${CMAKE_COMMAND} -S. -Bbuild -GNinja
      -DCMAKE_CXX_COMPILER=${cxx_compiler}
      -DCMAKE_C_COMPILER=${c_compiler} -DCMAKE_BUILD_TYPE=Debug -Dtolc_ROOT=${test_package})
  set_tests_properties(Generate${systemTestName} PROPERTIES FIXTURES_SETUP
                                                   FixtureGenerate${systemTestName})
  # Need to install package before generating
  set_tests_properties(Generate${systemTestName} PROPERTIES FIXTURES_REQUIRED
                                                   FixtureInstallPackage)

  # Build the system test project
  add_test(
    NAME Build${systemTestName}
    WORKING_DIRECTORY ${consumer}
    COMMAND ${CMAKE_COMMAND} --build ${consumer}/build)
  set_tests_properties(Build${systemTestName} PROPERTIES FIXTURES_SETUP
                                                FixtureBuild${systemTestName})
  # Need to generate the build files before building
  set_tests_properties(Build${systemTestName} PROPERTIES FIXTURES_REQUIRED
                                                FixtureGenerate${systemTestName})

  # This requires that the project creates an executable called 'consumer' in the build directory
  add_test(NAME Run${systemTestName}
           COMMAND ${consumer}/build/consumer)
  # Need to build the consumer before running it
  set_tests_properties(Run${systemTestName} PROPERTIES FIXTURES_REQUIRED
                                              FixtureBuild${systemTestName})

  add_test(NAME CleanUp${systemTestName}BuildDirectory
           COMMAND ${CMAKE_COMMAND} -E rm -rf
                   ${consumer}/build)
  # After running the final test, cleanup the build directory
  set_tests_properties(CleanUp${systemTestName}BuildDirectory PROPERTIES FIXTURES_CLEANUP
                                                                Run${systemTestName})
endforeach()
