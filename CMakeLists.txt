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

include(cmake/GetFrontendpy.cmake)
set(frontendpy_asset_name "Frontend.py-${CMAKE_SYSTEM_NAME}-master")
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(frontendpy_asset_name "${frontendpy_asset_name}-${CMAKE_BUILD_TYPE}")
endif()
get_frontend_py(ASSET_NAME ${frontendpy_asset_name}.tar.xz)
copy_frontend_py_docs(SRC_DIR ${frontend_py_SOURCE_DIR})

include(cmake/GetParser.cmake)
set(parser_asset_name "Parser-${CMAKE_SYSTEM_NAME}-master")
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(parser_asset_name "${parser_asset_name}-${CMAKE_BUILD_TYPE}")
endif()
get_parser(ASSET_NAME ${parser_asset_name}.tar.xz)

# If we are not building from source we need to create some variables
# containing the paths to the included system library
# Set the include path for the system library in the variable
# We are using the standard library shipped
# with the downloaded llvm for include paths in the parsing
set(PARSER_LLVM_DIR ${parser_SOURCE_DIR}/lib/Parser)
get_parser_system_include(
  VARIABLE
  SYSTEM_INCLUDE_PATHS
  PARSER_SOURCE_DIR
  ${parser_SOURCE_DIR}
  LIBCPP_ROOT_DIR
  ${parser_SOURCE_DIR}/lib/Parser
  LLVM_VERSION
  ${PARSER_LLVM_VERSION})

# These uppercase variables will be used when configuring getSystemIncludes.cpp
set(SYSTEM_INCLUDES "")
set(TOKEN_STRING "{replaceme}")
# Used as a root directory for system header files to be placed next to tolc executable
set(relativeIncludeDir system_include)
foreach(include ${SYSTEM_INCLUDE_PATHS})
  string(REPLACE ${parser_SOURCE_DIR}/lib/Parser
                 ${TOKEN_STRING}/${relativeIncludeDir} formated_include
                 ${include})
  list(APPEND SYSTEM_INCLUDES ${formated_include})
endforeach()

include(cmake/copyLLVMIncludes.cmake)
# Copy the llvm headers relative to the tolc executable
# (will always be ${CMAKE_CURRENT_BINARY_DIR} as long as
#  this is called in the same file as the add_executable)
copy_llvm_includes(
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/system_include LLVM_DIR
  ${PARSER_LLVM_DIR} LLVM_VERSION ${PARSER_LLVM_VERSION})

include(FetchContent)
FetchContent_Declare(
  cli11_content
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG v2.1.1)
FetchContent_Populate(cli11_content)

include(cmake/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE tolc_conan_profile)

find_conan_packages(REQUIRES spdlog/1.8.0 fmt/7.1.3 PROFILE
                    ${tolc_conan_profile})

include(cmake/GetBoost.cmake)
get_boost(VERSION 1.77.0)

configure_file(
  src/TolcInternal/getSystemIncludes.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/src/TolcInternal/getSystemIncludes.cpp @ONLY)

add_library(
  TolcInternal
  src/CommandLine/parse.cpp
  src/Log/postJSON.cpp
  src/Log/log.cpp
  src/TolcInternal/buildConfig.cpp
  src/TolcInternal/getExecutableDirectoryPath.cpp
  src/TolcInternal/getSystemIncludes.cpp
  src/TolcInternal/run.cpp)
target_include_directories(TolcInternal PUBLIC src)
target_include_directories(TolcInternal SYSTEM
                           PRIVATE ${cli11_content_SOURCE_DIR}/include)
target_link_libraries(TolcInternal PUBLIC Tolc::Parser Tolc::Frontend.py
                                          spdlog::spdlog fmt::fmt)
set_target_properties(TolcInternal PROPERTIES CXX_STANDARD_REQUIRED ON
                                              CXX_EXTENSIONS OFF)

add_warnings(TARGET TolcInternal)
add_options(TARGET TolcInternal)
target_link_boost(TARGET TolcInternal)
# Do not try to link against static libraries (only using headers for now)
target_compile_definitions(TolcInternal PRIVATE BOOST_ALL_NO_LIB)
# Prefer std::filesystem over boost::filesystem
target_compile_definitions(TolcInternal PRIVATE BOOST_DLL_USE_STD_FS)

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

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" OFF)
  if(ENABLE_PACKAGING)
    add_subdirectory(tools/packaging)
    if(ENABLE_TESTING)
      add_subdirectory(tests/packaging)
    endif()
  endif()
endif()
