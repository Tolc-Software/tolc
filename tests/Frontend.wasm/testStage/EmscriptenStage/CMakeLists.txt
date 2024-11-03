cmake_minimum_required(VERSION 3.25)

# NOTE: Configuring this project the following flag must be present:
# -DCMAKE_TOOLCHAIN_FILE=${path_to_emscripten_toolchain}

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(frontend_languages CXX)
else()
  set(frontend_languages C CXX)
endif()

project(
  EmscriptenStage
  VERSION 1.0
  LANGUAGES ${frontend_languages})

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Get the variable 'target_name' NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# Get the variable 'sources' NOTE: This will be generated for each test
include(cmake/Sources.cmake)

# Create the js module
add_executable(${target_name} ${sources})
target_include_directories(${target_name} PRIVATE src)

# Use C++17 since the tests are using std::optional etc.
set_target_properties(
  ${target_name}
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             CXX_STANDARD 17)

target_link_libraries(${target_name} PRIVATE embind)
# Export Promise as 'loadm' for module 'm' -s MODULARIZE=1 sets it as a promise
# based load Note that this is necessary for preJS to work properly
set_property(
  TARGET ${target_name}
  PROPERTY
    LINK_FLAGS
    "-s MODULARIZE=1 -s EXPORT_NAME=\"loadm\" --pre-js ${CMAKE_CURRENT_LIST_DIR}/src/pre.js "
)
