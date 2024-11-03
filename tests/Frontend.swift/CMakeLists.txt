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

find_package(Catch2 REQUIRED)
find_package(fmt REQUIRED)

include(FetchContent)
FetchContent_Declare(
  stage_content
  GIT_REPOSITORY https://github.com/Tolc-Software/Stage.git
  GIT_TAG main)
FetchContent_MakeAvailable(stage_content)

# Create a platform independent way of getting the path to the test stage
set(test_stage ${CMAKE_CURRENT_LIST_DIR}/testStage)
set(examples_dir ${PROJECT_SOURCE_DIR}/docs/examples)
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  # This is to normalize strings relative to how they are used in
  # std::filesystem
  string(REPLACE "/" "\\\\" test_stage ${test_stage})
  string(REPLACE "/" "\\\\" examples_dir ${examples_dir})
endif()
configure_file(
  TestUtil/include/TestStage/paths.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestStage/paths.cpp @ONLY)

# Everything put in TestUtil/include is available
add_library(
  Frontend.swift_TestUtil
  TestUtil/include/TestUtil/objcSwiftStage.cpp
  TestUtil/include/TestUtil/parse.cpp TestUtil/include/TestStage/paths.cpp)
target_include_directories(Frontend.swift_TestUtil PUBLIC TestUtil/include)
target_link_libraries(
  Frontend.swift_TestUtil PRIVATE Parser Frontend.swift Catch2::Catch2
                                  Tolc::Stage fmt::fmt)
target_link_libraries(Frontend.swift_TestUtil PUBLIC IR::IR)

add_warnings(TARGET Frontend.swift_TestUtil)
add_options(TARGET Frontend.swift_TestUtil)

if(ENABLE_COVERAGE)
  include(${CMAKE_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of frontend
  target_code_coverage(Frontend.swift)

  add_code_coverage_all_targets()
endif()

# Create a test fixture for downloading dependencies in the test stage
add_test(
  NAME ConfigureTestStage
  WORKING_DIRECTORY ${test_stage}
  COMMAND ${CMAKE_COMMAND} -S. -Bbuild -GNinja -Dlanguage=objc)
set_tests_properties(ConfigureTestStage PROPERTIES FIXTURES_SETUP TestStage)

foreach(
  test
  # Objc/Builders/enumBuilder.cpp ObjcSwift/Builders/attributeBuilder.cpp
  # ObjcSwift/Builders/classBuilder.cpp ObjcSwift/Builders/functionBuilder.cpp
  # ObjcSwift/Builders/moduleFileBuilder.cpp
  # ObjcSwift/Builders/typeToStringBuilder.cpp ObjcSwift/Helpers/combine.cpp
  # ObjcSwift/Proxy/attribute.cpp ObjcSwift/Proxy/class.cpp
  # ObjcSwift/Proxy/enum.cpp ObjcSwift/Proxy/function.cpp
  # ObjcSwift/Proxy/module.cpp ObjcSwift/Proxy/moduleFile.cpp
  ObjcSwift/Helpers/typeToStringBuilder.cpp
  # complex.cpp functional.cpp inheritence.cpp operators.cpp variants.cpp
  # virtualInheritence.cpp
  arrays.cpp
  builtins.cpp
  classes.cpp
  defaultArgNames.cpp
  deques.cpp
  documentationStyles.cpp
  enums.cpp
  filesystem_paths.cpp
  functions.cpp
  globalVariables.cpp
  libraryNSClash.cpp
  lists.cpp
  maps.cpp
  memberVariables.cpp
  namespaces.cpp
  optionals.cpp
  order.cpp
  overloadedFunctions.cpp
  pairs.cpp
  sets.cpp
  smartPointers.cpp
  templates.cpp
  tuples.cpp
  unordered_maps.cpp
  unordered_sets.cpp
  userDefined.cpp
  valarrays.cpp
  vectors.cpp)
  # Ex: ObjcSwift/Proxy/moduleFile.cpp -> moduleFile
  get_filename_component(name ${test} NAME_WLE)
  set(testName Frontend.swift_${name})

  add_executable(${testName} ${test})
  target_link_libraries(
    ${testName} PRIVATE Catch2::Catch2WithMain fmt::fmt Frontend.swift_TestUtil
                        Frontend.swift Tolc::Stage)

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
