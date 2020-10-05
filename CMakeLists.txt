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

find_conan_packages(
  REQUIRES CLI11/1.9.1@cliutils/stable PROFILE
  ${CMAKE_CURRENT_LIST_DIR}/tools/conan_profiles/${CMAKE_SYSTEM_NAME}/clang)
find_conan_packages(
  REQUIRES
  boost/1.74.0
  DO_NOT_FIND
  OPTIONS
  boost:zlib=False
  boost:bzip2=False
  PROFILE
  ${CMAKE_CURRENT_LIST_DIR}/tools/conan_profiles/${CMAKE_SYSTEM_NAME}/clang)
find_package(Boost REQUIRED)

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

  set(SYSTEM_INCLUDES "")
  set(TOKEN_STRING "{replaceme}")
  foreach(include ${SYSTEM_INCLUDE_PATHS})
    string(REPLACE ${parser_SOURCE_DIR}/lib/Parser ${TOKEN_STRING}
                   formated_include ${include})
    list(APPEND SYSTEM_INCLUDES ${formated_include})
  endforeach()
endif()

configure_file(
  src/TolcInternal/getSystemIncludes.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/src/TolcInternal/getSystemIncludes.cpp @ONLY)

add_library(
  TolcInternal
  src/TolcInternal/run.cpp src/TolcInternal/getExecutablePath.cpp
  src/TolcInternal/buildConfig.cpp src/CommandLine/parse.cpp
  src/TolcInternal/getSystemIncludes.cpp)
target_include_directories(TolcInternal PUBLIC src)
target_link_libraries(TolcInternal PRIVATE Tolc::Parser Tolc::Frontend.py
                                           CLI11::CLI11 Boost::Boost)
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
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()
endif()
