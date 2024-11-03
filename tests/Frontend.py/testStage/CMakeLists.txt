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

include(cmake/GetPybind11.cmake)
get_pybind11(VERSION 2.13.6)

# Get the variable 'target_name' NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# Get the variable 'sources' NOTE: This will be generated for each test
include(cmake/Sources.cmake)

# Create the python module
pybind11_add_module(${target_name} ${sources} SYSTEM)
target_include_directories(${target_name} PRIVATE src)

# Use C++17 since the tests are using std::optional etc.
set_target_properties(
  ${target_name}
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             CXX_STANDARD 17)
