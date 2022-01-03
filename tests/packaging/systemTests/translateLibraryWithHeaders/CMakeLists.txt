# Need 3.11 for FetchContent module
cmake_minimum_required(VERSION 3.11)

project(Consumer)

# NOTE: Assumes tolc package is put into following path before executing this test
if(NOT tolc_ROOT)
  message(
    FATAL_ERROR
      "Project ${PROJECT_NAME} did not recieve the tolc_ROOT variable. When configuring this project, inject it with -Dtolc_ROOT=..."
  )
endif()
find_package(tolc CONFIG PATHS ${tolc_ROOT})

if(NOT tolc_FOUND)
  message(
    FATAL_ERROR
      "No tolc package found! To test a specific version, place a prebuilt tolc package in ${tolc_ROOT}. This is normally done automatically."
  )
endif()

add_executable(consumer src/consumer.cpp)
set_target_properties(
  consumer
  PROPERTIES CXX_STANDARD_REQUIRED ON
             CXX_STANDARD 17
             CXX_EXTENSIONS OFF)
target_include_directories(consumer PUBLIC include)
target_include_directories(consumer PRIVATE src)

# These will be made available to the consumer executable,
# which will run the tests (see src/consumer.cpp)
set(public_headers "\"${PROJECT_SOURCE_DIR}/include/consumer.hpp\",
  \"${PROJECT_SOURCE_DIR}/include/SubDir/oneLevelDeep.hpp\"")
set(expected_out_file ${CMAKE_CURRENT_BINARY_DIR}/out/consumer.cpp)
set(expected_combined_header ${CMAKE_CURRENT_BINARY_DIR}/tolc/tolc_consumer.hpp)
configure_file(src/check_variables.hpp.in
               ${CMAKE_CURRENT_LIST_DIR}/src/check_variables.hpp @ONLY)

tolc_translate_target(
  TARGET
  consumer
  LANGUAGE
  python
  HEADERS
  include/consumer.hpp
  include/SubDir/oneLevelDeep.hpp
  DO_NOT_SEARCH_TARGET_INCLUDES
  OUTPUT
  ${CMAKE_CURRENT_BINARY_DIR}/out
  NO_ANALYTICS)
