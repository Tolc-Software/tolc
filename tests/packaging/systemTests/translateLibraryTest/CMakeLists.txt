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
target_include_directories(consumer PUBLIC include)
target_include_directories(consumer PRIVATE src)

tolc_translate_target(
  TARGET
  consumer
  LANGUAGE
  python
  OUTPUT_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/out)

set(expected_out_file ${CMAKE_CURRENT_BINARY_DIR}/out/consumer.cpp)
if(NOT EXISTS ${expected_out_file})
  message(
    FATAL_ERROR
      "Tolc did not create an output file. Expected file does not exist: ${expected_out_file}"
  )
endif()

set(expectedCombinedHeader ${CMAKE_CURRENT_BINARY_DIR}/tolc_consumer.hpp)
if(NOT EXISTS ${expectedCombinedHeader})
  message(FATAL_ERROR "The combined header does not exist. Expected: ${expectedCombinedHeader}")
endif()

# Check that it found all of the public headers
file(STRINGS ${expectedCombinedHeader} combinedHeaderLines)
list(LENGTH combinedHeaderLines numberOfHeadersFound)
set(expectedNumberOfHeaders 3)
if(NOT ${numberOfHeadersFound} EQUAL ${expectedNumberOfHeaders})
  message(FATAL_ERROR "Tolc CMake module did not find all of the public headers to combine. Expected to find ${expectedNumberOfHeaders}, but found ${numberOfHeadersFound}")
endif()
