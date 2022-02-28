# Need 3.11 for FetchContent module
cmake_minimum_required(VERSION 3.15)

project(Consumer)

# NOTE: Assumes tolc package is put into following path before executing this test
if(NOT tolc_DIR)
  message(
    FATAL_ERROR
      "Project ${PROJECT_NAME} did not recieve the tolc_DIR variable. When configuring this project, inject it with -Dtolc_DIR=..."
  )
endif()
find_package(tolc CONFIG PATHS ${tolc_DIR} REQUIRED)

add_executable(consumer src/consumer.cpp)

add_library(myLib src/myLib.cpp)
target_include_directories(myLib PUBLIC include)

tolc_create_bindings(
  TARGET
  myLib
  LANGUAGE
  wasm
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/out
  NO_ANALYTICS)

target_link_libraries(myLib_wasm PRIVATE embind)
