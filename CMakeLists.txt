cmake_minimum_required(VERSION 3.17)

project(
  tolc
  VERSION 0.1
  LANGUAGES CXX)

# Define add_{warnings,options}
include(cmake/WarningsAndOptions.cmake)

# Define options such as ccache
include(cmake/ProjectSettings.cmake)

include(cmake/Sanitizers.cmake)

include(cmake/StaticAnalyzers.cmake)

include(cmake/CodeCoverage.cmake)

# Define run_conan
include(cmake/ConanHelpers.cmake)
# This helps with packaging
set(CONAN_REQUIRES_PACKAGES CLI11/1.9.1@cliutils/stable)
find_conan_packages(REQUIRES ${CONAN_REQUIRES_PACKAGES})

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

add_library(TolcInternal src/TolcInternal/run.cpp
                         src/CommandLine/addOptions.cpp)
target_include_directories(TolcInternal PUBLIC src)
target_link_libraries(TolcInternal PRIVATE Tolc::Parser Tolc::Frontend.py
                                           CLI11::CLI11)
set_target_properties(TolcInternal PROPERTIES CXX_STANDARD_REQUIRED ON
                                              CXX_EXTENSIONS OFF)

add_warnings(TARGET TolcInternal)
add_options(TARGET TolcInternal)

add_executable(tolc src/tolc.main.cpp)
target_link_libraries(tolc PRIVATE TolcInternal)
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
