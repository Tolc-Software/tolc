include_guard()

# See the README for more detailed documentation

function(function_name)

endfunction()

function(add_system_test)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues NAME LANGUAGE)
  set(multiValues RUN_COMMAND)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})
  # Path to the consumer project
  set(consumer ${CMAKE_CURRENT_LIST_DIR}/${test_name})
  set(test_name ${ARG_NAME})
  set(language ${ARG_LANGUAGE})
  set(run_command ${ARG_RUN_COMMAND})

  set(extra_cmake "")
  if(language STREQUAL wasm)
    set(extra_cmake -DCMAKE_TOOLCHAIN_FILE=${emscripten_toolchain})
  endif()

  # Generate the consumer build files
  # NOTE: Here we inject the path to the package that gets tested
  add_test(
    NAME Generate${test_name}
    WORKING_DIRECTORY ${consumer}
    COMMAND ${CMAKE_COMMAND} -S. -Bbuild -G Ninja
            -Dtolc_DIR=${test_package}/lib/cmake/tolc ${extra_cmake})
  set_tests_properties(Generate${test_name}
                       PROPERTIES FIXTURES_SETUP FixtureGenerate${test_name})
  # Need to install package before generating
  set_tests_properties(Generate${test_name} PROPERTIES FIXTURES_REQUIRED
                                                       FixtureInstallPackage)

  # Build the system test project
  add_test(
    NAME Build${test_name}
    WORKING_DIRECTORY ${consumer}
    COMMAND ${CMAKE_COMMAND} --build build)
  set_tests_properties(Build${test_name} PROPERTIES FIXTURES_SETUP
                                                    FixtureBuild${test_name})
  # Need to generate the build files before building
  set_tests_properties(Build${test_name} PROPERTIES FIXTURES_REQUIRED
                                                    FixtureGenerate${test_name})

  # This requires that the project creates an executable called 'consumer' in the build directory
  add_test(NAME Run${test_name} COMMAND ${run_command})
  set_tests_properties(Run${test_name} PROPERTIES FIXTURES_SETUP
                                                  FixtureRun${test_name})
  # Need to build the consumer before running it
  set_tests_properties(Run${test_name} PROPERTIES FIXTURES_REQUIRED
                                                  FixtureBuild${test_name})

  add_test(NAME CleanUp${test_name} COMMAND ${CMAKE_COMMAND} -E rm -r -f
                                            ${consumer}/build)
  # After running the final test, cleanup the build directory
  set_tests_properties(CleanUp${test_name} PROPERTIES FIXTURES_REQUIRED
                                                      FixtureRun${test_name})
endfunction()

# Generate helpers for the tests
foreach(test_name translateFileTest translateLibraryWithHeaders
                  translateLibraryTest createTranslationTest)
  add_system_test(NAME ${test_name} LANGUAGE python RUN_COMMAND
                  ${CMAKE_CURRENT_LIST_DIR}/${test_name}/build/consumer)
endforeach()

foreach(test_name createTranslationTestWasm)
  add_system_test(
    NAME
    ${test_name}
    LANGUAGE
    wasm
    RUN_COMMAND
    node
    --no-experimental-fetch
    ${CMAKE_CURRENT_LIST_DIR}/${test_name}/test.js)
endforeach()
