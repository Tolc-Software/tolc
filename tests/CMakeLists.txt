include_guard()

include(FetchContent)
FetchContent_Declare(
  u_test
  GIT_REPOSITORY https://github.com/boost-ext/ut.git
  GIT_TAG v1.1.8)

FetchContent_MakeAvailable(u_test)

find_package(fmt REQUIRED)

add_library(TestUtil TestUtil/src/TestUtil/CommandLineInput.cpp)
target_include_directories(TestUtil PUBLIC TestUtil/include)

if(ENABLE_COVERAGE)
  include(${PROJECT_SOURCE_DIR}/cmake/CodeCoverage.cmake)

  # Use coverage of parser
  target_code_coverage(TolcInternal)
  add_code_coverage_all_targets()
endif()

include(${PROJECT_SOURCE_DIR}/cmake/Sanitizers.cmake)

foreach(testFile CommandLine/parse.cpp TolcInternal/getExecutablePath.cpp
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
