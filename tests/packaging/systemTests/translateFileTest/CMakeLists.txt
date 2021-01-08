cmake_minimum_required(VERSION 3.17)

project(Consumer)

# NOTE: Assumes tolc package is put into following path before executing this test
if(NOT tolc_ROOT)
  message(FATAL_ERROR "Project ${PROJECT_NAME} did not recieve the tolc_ROOT variable. When configuring this project, inject it with -Dtolc_ROOT=...")
endif()
find_package(tolc CONFIG PATHS ${tolc_ROOT})

if(NOT tolc_FOUND)
  message(FATAL_ERROR "No tolc package found! To test a specific version, place a prebuilt tolc package in ${tolc_ROOT}. This is normally done automatically.")
endif()

tolc_translate_file(
  MODULE_NAME
  consumer
  LANGUAGE
  python
  FILE
  include/consumer.hpp
  OUTPUT_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/out)

set(expected_out_file ${CMAKE_CURRENT_BINARY_DIR}/out/consumer.cpp)
if(NOT EXISTS ${expected_out_file})
  message(
    FATAL_ERROR
      "Tolc did not create an output file. Expected file does not exist: ${expected_out_file}"
  )
endif()

add_executable(consumer src/consumer.cpp)
target_include_directories(consumer PUBLIC include)
