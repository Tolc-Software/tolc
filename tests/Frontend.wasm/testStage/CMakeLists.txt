cmake_minimum_required(VERSION 3.25)

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(frontend_languages CXX)
else()
  set(frontend_languages C CXX)
endif()

project(
  TestStage
  VERSION 1.0
  LANGUAGES ${frontend_languages})

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/GetEmscripten.cmake)

set(emscripten_version 3.1.69)
if(NOT emsdk_SOURCE_DIR)
  get_emscripten(VERSION ${emscripten_version})
endif()

# Get the variable 'target_name' NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# Get the variable 'sources' NOTE: This will be generated for each test
include(cmake/Sources.cmake)

# Create the emscripten module This is only for the local test files in this
# directory. The EmscriptenStage uses the Emscripten Toolchain file instead
add_emscripten_library(
  TARGET
  ${target_name}
  SOURCES
  ${sources}
  EMSDK_PATH
  ${emsdk_SOURCE_DIR}
  VERSION
  ${emscripten_version})
