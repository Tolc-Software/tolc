cmake_minimum_required(VERSION 3.25)

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(tolc_languages CXX)
else()
  set(tolc_languages C CXX)
endif()

project(
  tolc
  VERSION 0.7.0
  LANGUAGES ${tolc_languages})

configure_file(docs/ReleaseNotes/version.in
               ${CMAKE_CURRENT_LIST_DIR}/docs/ReleaseNotes/version @ONLY)

# Define add_{warnings,options}
include(./cmake/WarningsAndOptions.cmake)
# Need a special options since the used version of Boost::asio doesn't support
# C++20 on MacOS
include(./cmake/Options.cmake)

# Define options such as ccache
include(./cmake/ProjectSettings.cmake)

include(./cmake/Sanitizers.cmake)

include(./cmake/StaticAnalyzers.cmake)

find_package(CLI11 REQUIRED)
find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(ctre REQUIRED)

include(./cmake/GetBoost.cmake)
get_boost(VERSION 1.77.0)

include(./cmake/LLVMUtilities.cmake)
# Download if necessary and get the clang libs Defines
# llvm_entry_{SOURCE_DIR,INCLUDE_DIR} variables LLVM-19.1.0-macOS-ARM64.tar.xz
set(llvm_entry_version 19.1.0)
set(llvm_entry_major_version 19)
setup_llvm(NAME "LLVM_ENTRY" VERSION ${llvm_entry_version})

# Macro to get downloaded Clang and llvm packages
get_clang_and_llvm(PATH ${llvm_entry_SOURCE_DIR})
# From the LLVM cmake module. Defines add_llvm_{executable,library}
include(AddLLVM)

# Defines get_system_include and format_includes
include(./cmake/IncludePathHelpers.cmake)

# Set the include path for the system library in the variable We are using the
# standard library shipped with the downloaded llvm for include paths in the
# parsing
get_system_include(VARIABLE SYSTEM_INCLUDE_PATHS LLVM_DIRECTORY
                   ${llvm_entry_SOURCE_DIR} LLVM_VERSION ${llvm_entry_version})

# Set the include path for the system library in the variable We are using the
# standard library shipped with the downloaded llvm for include paths in the
# parsing These uppercase variables will be used when configuring
# getSystemIncludes.cpp
set(SYSTEM_INCLUDES "")
set(TOKEN_STRING "{replaceme}")
# Used as a root directory for system header files to be placed next to tolc
# executable
set(relativeIncludeDir system_include)
foreach(include ${SYSTEM_INCLUDE_PATHS})
  string(REPLACE ${llvm_entry_SOURCE_DIR} ${TOKEN_STRING}/${relativeIncludeDir}
                 formatted_include ${include})
  list(APPEND SYSTEM_INCLUDES ${formatted_include})
endforeach()

format_includes(VARIABLE SYSTEM_INCLUDES INCLUDES ${SYSTEM_INCLUDES} SYSTEM)

include(./cmake/copyLLVMIncludes.cmake)
# Copy the llvm headers relative to the tolc executable (will always be
# ${CMAKE_CURRENT_BINARY_DIR} as long as this is called in the same file as the
# add_executable)
copy_llvm_includes(
  OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/system_include LLVM_DIR
  ${llvm_entry_SOURCE_DIR} LLVM_MAJOR_VERSION ${llvm_entry_major_version})

configure_file(
  src/TolcInternal/getSystemIncludes.cpp.in
  ${CMAKE_CURRENT_LIST_DIR}/src/TolcInternal/getSystemIncludes.cpp @ONLY)

configure_file(src/Parser/Helpers/commandLineArgs.cpp.in
               ${CMAKE_CURRENT_LIST_DIR}/src/Helpers/commandLineArgs.cpp @ONLY)

add_llvm_library(
  Parser
  src/Parser/Builders/commonBuilder.cpp
  src/Parser/Builders/enumBuilder.cpp
  src/Parser/Builders/fieldBuilder.cpp
  src/Parser/Builders/functionBuilder.cpp
  src/Parser/Builders/namespaceBuilder.cpp
  src/Parser/Builders/structBuilder.cpp
  src/Parser/Builders/typeBuilder.cpp
  src/Parser/Consumer/ParserConsumer.cpp
  src/Parser/Factory/ParserFrontendActionFactory.cpp
  src/Parser/Frontend/ParserFrontendAction.cpp
  src/Parser/Helpers/Type/Matchers/base.cpp
  src/Parser/Helpers/Type/Matchers/container.cpp
  src/Parser/Helpers/Type/Matchers/function.cpp
  src/Parser/Helpers/Type/matchers.cpp
  src/Parser/Helpers/Type/utilities.cpp
  src/Parser/Helpers/Utils/combine.cpp
  src/Parser/Helpers/Utils/split.cpp
  src/Parser/Helpers/Utils/string.cpp
  src/Parser/Helpers/commandLineArgs.cpp
  src/Parser/Helpers/getStructData.cpp
  src/Parser/Helpers/walkIRStructure.cpp
  src/Parser/Parser/Parse.cpp
  src/Parser/Parser/Windows/systemIncludeHelper.cpp
  src/Parser/Template/cache.cpp
  src/Parser/Visitor/Helpers/addId.cpp
  src/Parser/Visitor/Helpers/getDocumentation.cpp
  src/Parser/Visitor/Helpers/parseFunction.cpp
  src/Parser/Visitor/ParserVisitor.cpp
  src/Parser/Visitor/VisitCXXRecordDecl.cpp
  src/Parser/Visitor/VisitClassTemplateSpecializationDecl.cpp
  src/Parser/Visitor/VisitEnumDecl.cpp
  src/Parser/Visitor/VisitFieldDecl.cpp
  src/Parser/Visitor/VisitFunctionDecl.cpp
  src/Parser/Visitor/VisitFunctionTemplateDecl.cpp
  src/Parser/Visitor/VisitNamespaceDecl.cpp
  src/Parser/Visitor/VisitVarDecl.cpp)

target_link_libraries(
  Parser
  PUBLIC IR::IR
  PRIVATE clangASTMatchers clangBasic clangTooling spdlog::spdlog fmt::fmt
          ctre::ctre)

add_warnings(TARGET Parser)
add_options(TARGET Parser)

# Set sanitizer options on Parser if options defined
enable_sanitizers(Parser)

# Ignore warnings from {clang} as if they were system headers.
target_include_directories(Parser SYSTEM PRIVATE ${llvm_entry_INCLUDE_DIR})
target_include_directories(Parser PRIVATE src/Parser)
target_include_directories(
  Parser PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include/Parser>
                $<INSTALL_INTERFACE:include/Parser>)

set_target_properties(Parser PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                                 OFF)

include(./cmake/GetFrontend.cmake)
add_library(
  Frontend.py
  src/Frontend/Python/frontend.cpp
  src/Pybind/Builders/attributeBuilder.cpp
  src/Pybind/Builders/classBuilder.cpp
  src/Pybind/Builders/enumBuilder.cpp
  src/Pybind/Builders/functionBuilder.cpp
  src/Pybind/Builders/moduleBuilder.cpp
  src/Pybind/Builders/moduleFileBuilder.cpp
  src/Pybind/Builders/typeToStringBuilder.cpp
  src/Pybind/Helpers/getDocumentationParameter.cpp
  src/Pybind/Helpers/operatorNames.cpp
  src/Pybind/Helpers/split.cpp
  src/Pybind/Helpers/string.cpp
  src/Pybind/Helpers/trampolineClass.cpp
  src/Pybind/Helpers/types.cpp
  src/Pybind/Proxy/attribute.cpp
  src/Pybind/Proxy/class.cpp
  src/Pybind/Proxy/enum.cpp
  src/Pybind/Proxy/function.cpp
  src/Pybind/Proxy/module.cpp
  src/Pybind/Proxy/moduleFile.cpp
  src/Pybind/checkType.cpp
  src/Pybind/getOverloadedFunctions.cpp
  src/Pybind/returnValuePolicy.cpp)

add_warnings(TARGET Frontend.py)
add_options(TARGET Frontend.py)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.py)

target_link_libraries(
  Frontend.py
  PUBLIC IR::IR
  PRIVATE fmt::fmt spdlog::spdlog)

target_include_directories(Frontend.py PRIVATE src)
target_include_directories(
  Frontend.py PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                     $<INSTALL_INTERFACE:include>)

set_target_properties(Frontend.py PROPERTIES CXX_STANDARD_REQUIRED ON
                                             CXX_EXTENSIONS OFF)

# copy_frontend_docs(NAME Frontend.py SRC_DIR ${frontend.py_SOURCE_DIR} COPY_TO
# ${CMAKE_CURRENT_LIST_DIR}/docs/packaging/docs/python)

add_library(
  Frontend.wasm
  src/Embind/Builders/attributeBuilder.cpp
  src/Embind/Builders/classBuilder.cpp
  src/Embind/Builders/enumBuilder.cpp
  src/Embind/Builders/functionBuilder.cpp
  src/Embind/Builders/moduleBuilder.cpp
  src/Embind/Builders/moduleFileBuilder.cpp
  src/Embind/Builders/typeToStringBuilder.cpp
  src/Embind/Proxy/attribute.cpp
  src/Embind/Proxy/class.cpp
  src/Embind/Proxy/enum.cpp
  src/Embind/Proxy/function.cpp
  src/Embind/Proxy/module.cpp
  src/Embind/Proxy/moduleFile.cpp
  src/Frontend/Wasm/frontend.cpp
  src/Embind/checkType.cpp
  src/Embind/createFunctions.cpp
  src/Embind/getOverloadedFunctions.cpp
  src/Embind/Helpers/split.cpp
  src/Embind/Helpers/string.cpp
  src/Embind/Helpers/trampolineClass.cpp
  src/Embind/Helpers/types.cpp)

add_warnings(TARGET Frontend.wasm)
add_options(TARGET Frontend.wasm)

# Set the sanitizer options on project_options
enable_sanitizers(Frontend.wasm)

target_link_libraries(
  Frontend.wasm
  PUBLIC IR::IR
  PRIVATE fmt::fmt spdlog::spdlog)

target_include_directories(Frontend.wasm PRIVATE src)
target_include_directories(
  Frontend.wasm PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                       $<INSTALL_INTERFACE:include>)

set_target_properties(Frontend.wasm PROPERTIES CXX_STANDARD_REQUIRED ON
                                               CXX_EXTENSIONS OFF)
# copy_frontend_docs( NAME Frontend.wasm SRC_DIR ${frontend.wasm_SOURCE_DIR}
# COPY_TO ${CMAKE_CURRENT_LIST_DIR}/docs/packaging/docs/webassembly)

get_frontend_from_source(NAME Frontend.swift BRANCH feature/arm)
# copy_frontend_docs(NAME Frontend.swift SRC_DIR ${frontend.swift_SOURCE_DIR}
# COPY_TO ${CMAKE_CURRENT_LIST_DIR}/docs/packaging/docs/objc)

add_library(
  TolcInternal
  src/CommandLine/parse.cpp
  # src/Log/postJSON.cpp
  src/Log/log.cpp
  src/TolcInternal/buildConfig.cpp
  src/TolcInternal/getExecutableDirectoryPath.cpp
  src/TolcInternal/getSystemIncludes.cpp
  src/TolcInternal/run.cpp)
target_include_directories(TolcInternal PUBLIC src)
target_link_libraries(
  TolcInternal
  PUBLIC Parser
         Frontend.py
         Frontend.wasm
         Tolc::Frontend.swift
         spdlog::spdlog
         fmt::fmt
         CLI11::CLI11)
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
  option(ENABLE_TESTING "Build the testing tree" ON)
  if(ENABLE_TESTING)
    enable_testing()
    add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" ON)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()

  option(ENABLE_PACKAGING "Enables install and packaging via CPack" ON)
  if(ENABLE_PACKAGING)
    add_subdirectory(tools/packaging)
    if(ENABLE_TESTING)
      add_subdirectory(tests/packaging)
    endif()
  endif()
endif()
