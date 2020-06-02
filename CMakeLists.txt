cmake_minimum_required(VERSION 3.17)

project(
  tolc
  VERSION 0.1
  LANGUAGES CXX)

# Define project_warnings and project_options targets
include(cmake/WarningsAndOptions.cmake)
add_warnings(TARGET tolc_warnings)
add_options(TARGET tolc_options)

# Define options such as ccache
include(cmake/ProjectSettings.cmake)

include(cmake/Sanitizers.cmake)

include(cmake/StaticAnalyzers.cmake)

include(cmake/CodeCoverage.cmake)

# Define run_conan
include(cmake/ConanHelpers.cmake)
run_conan(REQUIRES lyra/1.4.0@_/_)

include(FetchContent)
FetchContent_Declare(
  Parser
  GIT_REPOSITORY git@github.com:srydell/Parser.git
  GIT_TAG master)
FetchContent_MakeAvailable(Parser)

FetchContent_Declare(
  PythonFrontend
  GIT_REPOSITORY git@github.com:srydell/frontend.py.git
  GIT_TAG master)
FetchContent_MakeAvailable(PythonFrontend)

add_library(CommandLineParser src/CommandLine/buildConfig.cpp)
target_include_directories(CommandLineParser PUBLIC src)
target_link_libraries(
  CommandLineParser
  PUBLIC tolc_warnings tolc_options Tolc::Parser Tolc::Frontend.py
  PRIVATE CONAN_PKG::lyra)
set_target_properties(CommandLineParser PROPERTIES CXX_STANDARD_REQUIRED ON
                                                   CXX_EXTENSIONS OFF)

add_executable(tolc src/tolc.main.cpp)
target_link_libraries(tolc PRIVATE CommandLineParser)
set_target_properties(tolc PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                               OFF)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    # TODO: Add tests
    # enable_testing()
    # add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()
endif()
