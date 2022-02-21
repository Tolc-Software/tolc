include_guard()

# Defines create_catch2_test and get_catch2
include(${modules}/Catch2Helpers.cmake)

get_catch2(VERSION v2.13.8)

find_package(fmt REQUIRED)

# Make sure the Parser in the tests can find the standard library headers
include(${modules}/copyLLVMIncludes.cmake)
copy_llvm_includes(
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/system_include LLVM_DIR
  ${PARSER_LLVM_DIR} LLVM_VERSION ${PARSER_LLVM_VERSION})

set(TEST_FILES_DIR ${CMAKE_CURRENT_LIST_DIR}/TestFiles)
set(TEST_PYTHON_DIR ${CMAKE_CURRENT_LIST_DIR}/TestUtil/python)
configure_file(
  TestUtil/include/TestUtil/getTestFilesDirectory.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/getTestFilesDirectory.hpp
  @ONLY)
add_library(TestUtil TestUtil/src/TestUtil/CommandLineInput.cpp
                     TestUtil/src/TestUtil/startServer.cpp)
add_warnings(TARGET TestUtil)
tolc_add_options(TARGET TestUtil)
target_include_directories(TestUtil PUBLIC TestUtil/include)
find_package(Threads REQUIRED)
target_link_libraries(TestUtil PRIVATE fmt::fmt Threads::Threads)
target_link_boost(TARGET TestUtil)

if(ENABLE_COVERAGE)
  include(${modules}/CodeCoverage.cmake)

  # Use coverage of tolc libs
  target_code_coverage(TolcInternal)
  add_code_coverage_all_targets()
endif()

include(${modules}/Sanitizers.cmake)

foreach(
  testFile
  CommandLine/parse.cpp Log/postJSON.cpp TolcInternal/buildConfig.cpp
  TolcInternal/run.cpp TolcInternal/getExecutableDirectoryPath.cpp
  TolcInternal/getSystemIncludes.cpp)

  # Ex: CommandLine/parse.cpp -> parse
  get_filename_component(testName ${testFile} NAME_WLE)

  create_catch2_test(
    TARGET
    ${testName}
    SOURCES
    ${testFile}
    INCLUDE
    ${PROJECT_SOURCE_DIR}/src
    LINK_LIBRARIES
    TestUtil
    TolcInternal
    fmt::fmt)

  add_warnings(TARGET ${testName})
  tolc_add_options(TARGET ${testName})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})
endforeach()
