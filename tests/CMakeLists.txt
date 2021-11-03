include_guard()

include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.12.1)
FetchContent_MakeAvailable(Catch2)

# These are needed to use create_catch2_test
set(Catch2_FOUND TRUE)
set(Catch2_INCLUDE_DIRS ${Catch2_SOURCE_DIR}/single_include)

# Defines create_catch2_test
include(${PROJECT_SOURCE_DIR}/cmake/SetupTest.cmake)

find_package(fmt REQUIRED)

# Make sure the Parser in the tests can find the standard library headers
include(${PROJECT_SOURCE_DIR}/cmake/copyLLVMIncludes.cmake)
copy_llvm_includes(
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/system_include LLVM_DIR
  ${PARSER_LLVM_DIR} LLVM_VERSION ${PARSER_LLVM_VERSION})

set(TEST_FILES_DIR ${CMAKE_CURRENT_LIST_DIR}/TestFiles)
set(TEST_PYTHON_DIR ${CMAKE_CURRENT_LIST_DIR}/TestUtil/python)
configure_file(
  TestUtil/include/TestUtil/getTestFilesDirectory.hpp.in
  ${CMAKE_CURRENT_LIST_DIR}/TestUtil/include/TestUtil/getTestFilesDirectory.hpp
  @ONLY)
add_library(TestUtil TestUtil/src/TestUtil/CommandLineInput.cpp TestUtil/src/TestUtil/startServer.cpp)
add_warnings(TARGET TestUtil)
add_options(TARGET TestUtil)
target_include_directories(TestUtil PUBLIC TestUtil/include)
find_package(Threads REQUIRED)
target_link_libraries(TestUtil PRIVATE fmt::fmt Threads::Threads)
target_link_boost(TARGET TestUtil)

if(ENABLE_COVERAGE)
  include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of tolc libs
  target_code_coverage(TolcInternal)
  add_code_coverage_all_targets()
endif()

include(${PROJECT_SOURCE_DIR}/cmake/Sanitizers.cmake)

foreach(
  testFile
  CommandLine/parse.cpp Log/postJSON.cpp TolcInternal/buildConfig.cpp TolcInternal/run.cpp
  TolcInternal/getExecutableDirectoryPath.cpp
  TolcInternal/getSystemIncludes.cpp)

  # Ex: CommandLine/parse.cpp -> parse
  get_filename_component(testName ${testFile} NAME_WLE)

  setup_test(
    TEST_NAME
    ${testName}
    SOURCE
    ${testFile}
    LIBRARIES
    TestUtil
    TolcInternal
    fmt::fmt)

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})
endforeach()

