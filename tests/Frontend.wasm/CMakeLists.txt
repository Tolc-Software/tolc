include_guard()

# Set the include path for the system library in the variable We are using the
# standard library shipped with the downloaded llvm for include paths in the
# parsing
get_system_include(VARIABLE SYSTEM_INCLUDE_PATHS LLVM_DIRECTORY
                   ${llvm_entry_SOURCE_DIR} LLVM_VERSION ${llvm_entry_version})

format_includes(VARIABLE SYSTEM_INCLUDE_PATHS INCLUDES ${SYSTEM_INCLUDE_PATHS}
                SYSTEM)

# If Parser was built from source, the file should only be expanded with empty
# strings and therefore use the default config from Parser.
configure_file(
  TestUtil/include/TestUtil/parserConfig.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/parserConfig.hpp @ONLY)

include(FetchContent)
FetchContent_Declare(
  stage_content
  GIT_REPOSITORY https://github.com/Tolc-Software/Stage.git
  GIT_TAG main)
FetchContent_MakeAvailable(stage_content)

find_package(Catch2 REQUIRED)

# Create a platform independent way of getting the path to the test stage
set(test_stage ${CMAKE_CURRENT_LIST_DIR}/testStage)
set(examples_dir ${PROJECT_SOURCE_DIR}/docs/examples)
set(npm_exe npm)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  # This is to normalize strings relative to how they are used in
  # std::filesystem
  string(REPLACE "/" "\\\\" test_stage ${test_stage})
  string(REPLACE "/" "\\\\" examples_dir ${examples_dir})
  # On windows, the actual command that will be executed is npm.cmd
  # https://stackoverflow.com/questions/47224093/ctest-gives-not-run-and-bad-command-although-commands-work-directly
  set(npm_exe npm.cmd)
endif()
configure_file(
  TestUtil/include/TestStage/paths.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestStage/paths.cpp @ONLY)

# Everything put in TestUtil/include is available
add_library(
  Frontend.wasm_TestUtil
  TestUtil/include/TestUtil/embindStage.cpp TestUtil/include/TestUtil/parse.cpp
  TestUtil/include/TestStage/paths.cpp)
target_include_directories(Frontend.wasm_TestUtil PUBLIC TestUtil/include)
target_link_libraries(Frontend.wasm_TestUtil PRIVATE Frontend.wasm Parser
                                                     fmt::fmt Catch2::Catch2)
target_link_libraries(Frontend.wasm_TestUtil PUBLIC IR::IR Tolc::Stage)
find_package(fmt REQUIRED)

add_warnings(TARGET Frontend.wasm_TestUtil)
add_options(TARGET Frontend.wasm_TestUtil)

if(ENABLE_COVERAGE)
  include(${CMAKE_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of frontend
  target_code_coverage(Frontend.wasm)

  add_code_coverage_all_targets()
endif()

# Before configuring the stage, we have to download node dependencies
add_test(
  NAME NpmInstall
  WORKING_DIRECTORY ${test_stage}/EmscriptenStage
  COMMAND ${npm_exe} install)
set_tests_properties(NpmInstall PROPERTIES FIXTURES_SETUP PreTestStage)

# Create a test fixture for downloading dependencies in the test stage
add_test(
  NAME ConfigureTestStage
  WORKING_DIRECTORY ${test_stage}
  COMMAND ${CMAKE_COMMAND} -S. -Bbuild -GNinja)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_SETUP TestStage)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_REQUIRED
                                                   PreTestStage)

foreach(
  test
  Embind/Builders/attributeBuilder.cpp
  Embind/Builders/classBuilder.cpp
  Embind/Builders/enumBuilder.cpp
  Embind/Builders/functionBuilder.cpp
  Embind/Builders/moduleFileBuilder.cpp
  Embind/Builders/typeToStringBuilder.cpp
  Embind/checkType.cpp
  Embind/Helpers/combine.cpp
  Embind/Helpers/string.cpp
  Embind/Proxy/attribute.cpp
  Embind/Proxy/class.cpp
  Embind/Proxy/enum.cpp
  Embind/Proxy/function.cpp
  Embind/Proxy/module.cpp
  Embind/Proxy/moduleFile.cpp
  arrays.cpp
  classes.cpp
  # complex.cpp deques.cpp
  enums.cpp
  # filesystem_paths.cpp functional.cpp
  functions.cpp
  globalVariables.cpp
  # lists.cpp
  maps.cpp
  inheritence.cpp
  namespaceRenaming.cpp
  namespaces.cpp
  # optionals.cpp overloadedFunctions.cpp
  pairs.cpp
  # sets.cpp
  smartPointers.cpp
  # templates.cpp
  tuples.cpp
  # unordered_maps.cpp unordered_sets.cpp userDefined.cpp valarrays.cpp
  # variants.cpp
  vectors.cpp
  virtualInheritence.cpp)
  # Ex: Embind/Proxy/moduleFile.cpp -> moduleFile
  get_filename_component(name ${test} NAME_WLE)
  set(testName Frontend.wasm_${name})

  add_executable(${testName} ${test})
  target_link_libraries(
    ${testName} PRIVATE Catch2::Catch2WithMain fmt::fmt Frontend.wasm_TestUtil
                        Frontend.wasm)

  target_include_directories(${testName} PRIVATE ${PROJECT_SOURCE_DIR}/src)
  set_target_properties(${testName} PROPERTIES CXX_STANDARD_REQUIRED ON
                                               CXX_EXTENSIONS OFF)

  add_test(NAME ${testName} COMMAND ${testName})

  add_warnings(TARGET ${testName})
  add_options(TARGET ${testName})

  set(ENABLE_SANITIZER_ADDRESS FALSE)
  set(ENABLE_SANITIZER_MEMORY FALSE)
  enable_sanitizers(${testName})

  string(FIND "${test}" "/" subdirectory)
  if(subdirectory EQUAL -1)
    # The test is in the root => should be an integration test

    # Require that the dependencies are downloaded so they can be copied to a
    # temporary test stage
    set_tests_properties(${testName} PROPERTIES FIXTURES_REQUIRED TestStage)
  endif()
endforeach()
