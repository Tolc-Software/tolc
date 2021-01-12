# Need 3.11 for FetchContent module
cmake_minimum_required(VERSION 3.11)

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

tolc_add_library(
  TARGET
  myPythonModule
  LANGUAGE
  python
  INPUT
  src/myPythonModule.cpp)
