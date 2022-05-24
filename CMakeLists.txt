cmake_minimum_required(VERSION 3.17)

project(
  tolc
  VERSION 0.6.2
  LANGUAGES CXX)

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

include(FetchContent)
FetchContent_Declare(
  cmake_modules
  GIT_REPOSITORY https://github.com/Tolc-Software/cmake-modules.git
  GIT_TAG main)
FetchContent_MakeAvailable(cmake_modules)
# Basis of all shared CMake modules
set(modules ${cmake_modules_SOURCE_DIR}/cmake)

# Define add_{warnings,options}
include(${modules}/WarningsAndOptions.cmake)
# Need a special options since the used version of Boost::asio
# doesn't support C++20 on MacOS
include(cmake/Options.cmake)

# Define options such as ccache
include(${modules}/ProjectSettings.cmake)

include(${modules}/Sanitizers.cmake)

include(${modules}/StaticAnalyzers.cmake)

include(${modules}/GetFrontend.cmake)
get_frontend(NAME Frontend.py VERSION v0.6.1)
copy_frontend_docs(NAME Frontend.py SRC_DIR ${frontend.py_SOURCE_DIR} COPY_TO
                   ${CMAKE_CURRENT_LIST_DIR}/docs/packaging/docs/python)

get_frontend(NAME Frontend.wasm VERSION v0.5.1)
copy_frontend_docs(
  NAME Frontend.wasm SRC_DIR ${frontend.wasm_SOURCE_DIR} COPY_TO
  ${CMAKE_CURRENT_LIST_DIR}/docs/packaging/docs/webassembly)

include(${modules}/GetParser.cmake)
get_parser(VERSION v0.5.6)

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

include(${modules}/copyLLVMIncludes.cmake)
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

include(${modules}/ConanHelpers.cmake)
# Create the profile from variables known during configure
setup_conan_profile(VARIABLE tolc_conan_profile)

find_conan_packages(REQUIRES spdlog/1.9.2 fmt/8.1.1 PROFILE
                    ${tolc_conan_profile})

include(${modules}/GetBoost.cmake)
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
target_link_libraries(
  TolcInternal PUBLIC Tolc::Parser Tolc::Frontend.py Tolc::Frontend.wasm
                      spdlog::spdlog fmt::fmt)
set_target_properties(TolcInternal PROPERTIES CXX_STANDARD_REQUIRED ON
                                              CXX_EXTENSIONS OFF)

add_warnings(TARGET TolcInternal)
tolc_add_options(TARGET TolcInternal)
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
