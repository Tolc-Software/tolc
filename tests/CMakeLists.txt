include_guard()

find_package(fmt REQUIRED)
find_package(Catch2 REQUIRED)

# Make sure the Parser in the tests can find the standard library headers
include(${CMAKE_SOURCE_DIR}/cmake/copyLLVMIncludes.cmake)
copy_llvm_includes(
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/system_include LLVM_DIR
  ${llvm_entry_SOURCE_DIR} LLVM_MAJOR_VERSION ${llvm_entry_major_version})

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
  include(${CMAKE_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of tolc libs
  target_code_coverage(TolcInternal)
  add_code_coverage_all_targets()
endif()

include(${CMAKE_SOURCE_DIR}/cmake/Sanitizers.cmake)

foreach(
  test
  CommandLine/parse.cpp Log/postJSON.cpp TolcInternal/buildConfig.cpp
  TolcInternal/run.cpp TolcInternal/getExecutableDirectoryPath.cpp
  TolcInternal/getSystemIncludes.cpp)

  # Ex: CommandLine/parse.cpp -> parse
  get_filename_component(testName ${test} NAME_WLE)

  add_executable(${testName} ${test})
  target_link_libraries(${testName} PRIVATE Catch2::Catch2WithMain fmt::fmt
                                            TolcInternal TestUtil)

  target_include_directories(${testName} PRIVATE ${PROJECT_SOURCE_DIR}/src)
  set_target_properties(${testName} PROPERTIES CXX_STANDARD_REQUIRED ON
                                               CXX_EXTENSIONS OFF)

  add_test(NAME ${testName} COMMAND ${testName})

  add_warnings(TARGET ${testName})
  tolc_add_options(TARGET ${testName})

  set(ENABLE_SANITIZER_ADDRESS TRUE)
  set(ENABLE_SANITIZER_MEMORY TRUE)
  enable_sanitizers(${testName})
endforeach()
