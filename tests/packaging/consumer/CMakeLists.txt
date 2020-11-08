cmake_minimum_required(VERSION 3.17)

project(
  consumer
  VERSION 0.1
  LANGUAGES CXX)

set(tolc_ROOT ${CMAKE_CURRENT_LIST_DIR}/build/tolc)
find_package(tolc CONFIG PATHS ${tolc_ROOT} REQUIRED)

tolc_translate_file(
  MODULE_NAME
  consumer
  LANGUAGE
  python
  FILE
  include/consumer.hpp
  OUTPUT_DIR
  ${CMAKE_CURRENT_BINARY_DIR}/out
  INCLUDES
  include)

if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/out/consumer.cpp)
  message(
    FATAL_ERROR
      "Tolc did not create an output file. Expected file does not exist: ${CMAKE_CURRENT_BINARY_DIR}/out/consumer.cpp"
  )
endif()

add_executable(consumer src/consumer.cpp)
target_include_directories(consumer PUBLIC include)
