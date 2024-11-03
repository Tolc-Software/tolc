cmake_minimum_required(VERSION 3.17)

# Temporary workaround for clang 14
# https://github.com/llvm/llvm-project/issues/53950
if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
  set(parser_languages CXX)
else()
  set(parser_languages C CXX)
endif()

project(
  consumer
  VERSION 0.1
  LANGUAGES ${parser_languages})

# Set /MT (or /MTd if debug) in MSVC-like compilers
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Used to tell which runtime lib to link with." FORCE)

set(FETCHCONTENT_FULLY_DISCONNECTED
    ON
    CACHE BOOL
          "Do not try to download dependencies (assume they are copied in)")
set(Parser_ROOT ${CMAKE_CURRENT_LIST_DIR}/build/parser)
find_package(Parser REQUIRED CONFIG PATHS ${Parser_ROOT} REQUIRED)

add_executable(consumer src/consumer.cpp)
target_link_libraries(consumer PRIVATE Tolc::Parser)
