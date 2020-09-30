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

include(cmake/GetFrontendpy.cmake)
get_frontend_py(ASSET_NAME Frontend.py-${CMAKE_SYSTEM_NAME}-master.tar.gz)

include(cmake/GetParser.cmake)

option(BUILD_DEPENDENCIES_FROM_SOURCE
       "Building dependencies from source instead of downloading an asset"
       FALSE)
set(parser_source_switch "")
if(BUILD_DEPENDENCIES_FROM_SOURCE)
  set(parser_source_switch "BUILD_FROM_SOURCE")
endif()
get_parser(ASSET_NAME Parser-${CMAKE_SYSTEM_NAME}-master.tar.gz
           ${parser_source_switch})

# If we are not building from source we need to create some variables
# containing the paths to the included system library
if(NOT parser_source_switch)
  # Set the include path for the system library in the variable
  # We are using the standard library shipped
  # with the downloaded llvm for include paths in the parsing
  get_parser_system_include(
    VARIABLE
    SYSTEM_INCLUDE_PATHS
    PARSER_SOURCE_DIR
    ${parser_SOURCE_DIR}
    LIBCPP_ROOT_DIR
    ${parser_SOURCE_DIR}/lib/Parser
    LLVM_VERSION
    ${PARSER_LLVM_VERSION})
endif()

add_library(
  TolcInternal src/TolcInternal/run.cpp src/TolcInternal/buildConfig.cpp
               src/CommandLine/parse.cpp)
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
