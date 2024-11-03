cmake_minimum_required(VERSION 3.15)

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

if(NOT language)
  message(
    FATAL_ERROR
      "language not set. Define it while configuring this project with -Dlanguage=objc"
  )
endif()

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_language(OBJC)
enable_language(OBJCXX)
enable_language(Swift)

# Get the variable 'target_name' NOTE: This will be generated for each test
include(cmake/TargetName.cmake)

# The users C++ library
add_library(${target_name} INTERFACE)
target_include_directories(${target_name} INTERFACE include)

# Use C++17 since the tests are using std::optional etc.
set_target_properties(
  ${target_name}
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS OFF
             CXX_STANDARD 17)

if("${language}" STREQUAL swift)
  # When outputting Swift, the middle Objective-C layer is "hidden"
  set(objc_target _${target_name}_objc)
else()
  set(objc_target ${target_name}_objc)
endif()

# Where the source files live
set(src src-${language})

# The Objective-C++ middle layer
add_library(${objc_target} ${src}/${objc_target}.mm)
target_include_directories(${objc_target} PUBLIC ${src})
target_link_libraries(${objc_target} PRIVATE ${target_name})

# NOTE: Take the CXX version of the incoming library and use it as the standard
set_property(TARGET ${objc_target} PROPERTY OBJCXX_STANDARD 17)

include(CTest)

if("${language}" STREQUAL swift)
  # The ObjcSwift middle layer
  set(swift_target ${target_name}_swift)
  add_library(${swift_target} ${src}/${swift_target}.swift)
  target_link_libraries(${swift_target} PRIVATE ${objc_target})
  set_property(
    TARGET ${swift_target}
    APPEND_STRING
    PROPERTY
      COMPILE_FLAGS
      "-import-objc-header ${CMAKE_CURRENT_LIST_DIR}/${src}/${objc_target}-Bridging-Header.h"
  )
  set_target_properties(${swift_target} PROPERTIES OUTPUT_NAME ${target_name})

  # The ObjcSwift exposed layer
  add_executable(swifty tests/main.swift)
  target_link_libraries(swifty PRIVATE ${swift_target})
  add_test(NAME swifty COMMAND swifty)

else()
  # # The Objective-C exposed layer
  add_executable(objecty tests/main.mm)
  target_link_libraries(objecty PRIVATE ${objc_target})
  target_link_libraries(objecty PRIVATE "-framework Foundation")
  add_test(NAME objecty COMMAND objecty)
endif()
