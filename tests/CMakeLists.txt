include_guard()

include(FetchContent)
FetchContent_Declare(
  u_test
  GIT_REPOSITORY https://github.com/boost-ext/ut.git
  GIT_TAG v1.1.8)

FetchContent_MakeAvailable(u_test)

find_package(fmt REQUIRED)

# Make sure the Parser in the tests can find the standard library headers
include(${PROJECT_SOURCE_DIR}/cmake/copyLLVMIncludes.cmake)
copy_llvm_includes(
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/system_include LLVM_DIR
  ${PARSER_LLVM_DIR} LLVM_VERSION ${PARSER_LLVM_VERSION})

set(TEST_FILES_DIR ${CMAKE_CURRENT_LIST_DIR}/TestFiles)
configure_file(
  TestUtil/include/TestUtil/getTestFilesDirectory.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/getTestFilesDirectory.hpp
  @ONLY)
add_library(TestUtil TestUtil/src/TestUtil/CommandLineInput.cpp)
target_include_directories(TestUtil PUBLIC TestUtil/include)

if(ENABLE_COVERAGE)
  include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of parser
  target_code_coverage(TolcInternal)
  add_code_coverage_all_targets()
endif()

include(${PROJECT_SOURCE_DIR}/cmake/Sanitizers.cmake)

foreach(
  testFile
  CommandLine/parse.cpp TolcInternal/buildConfig.cpp TolcInternal/run.cpp
  TolcInternal/getExecutableDirectoryPath.cpp
  TolcInternal/getSystemIncludes.cpp)

  # Ex: CommandLine/parse.cpp -> parse
  get_filename_component(testName ${testFile} NAME_WLE)

  add_executable(${testName} ${testFile})

  # Don't parse boost::ut for diagnostics
  get_target_property(utIncludes boost::ut INTERFACE_INCLUDE_DIRECTORIES)
  target_include_directories(${testName} SYSTEM PRIVATE ${utIncludes})

  target_link_libraries(${testName} PRIVATE boost::ut TolcInternal TestUtil)

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})

  add_test(NAME ${testName} COMMAND ${testName})
endforeach()
