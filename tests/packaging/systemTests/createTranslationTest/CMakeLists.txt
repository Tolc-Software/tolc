# Need 3.11 for FetchContent module
cmake_minimum_required(VERSION 3.15)

project(Consumer)

# NOTE: Assumes tolc package is put into following path before executing this test
if(NOT tolc_ROOT)
  message(FATAL_ERROR "Project ${PROJECT_NAME} did not recieve the tolc_ROOT variable. When configuring this project, inject it with -Dtolc_ROOT=...")
endif()
find_package(tolc CONFIG PATHS ${tolc_ROOT})

if(NOT tolc_FOUND)
  message(FATAL_ERROR "No tolc package found! To test a specific version, place a prebuilt tolc package in ${tolc_ROOT}. This is normally done automatically.")
endif()

add_executable(consumer src/consumer.cpp)

add_library(myLib src/myLib.cpp)
target_include_directories(myLib PUBLIC include)

tolc_create_translation(
  TARGET
  myLib
  LANGUAGE
  python
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/out)

# Generate helpers for the tests
find_package(Python3 REQUIRED)
# NOTE: If this fails, the test automatically fails
add_custom_target(
  importCreatedPyLib ALL
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tolc
  COMMAND ${Python3_EXECUTABLE} -c \"import myLib\"
)
# Make sure it only runs after myLib_python is built
add_dependencies(importCreatedPyLib myLib_python)
