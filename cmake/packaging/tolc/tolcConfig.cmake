include_guard()

function(find_tolc)
  # Go three level up in the package directory structure
  # Assuming something like
  #   <install>/bin/tolc
  #   <install>/lib/cmake/tolc/tolcConfig.cmake
  get_filename_component(cmake_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
  get_filename_component(lib_dir ${cmake_dir} DIRECTORY)
  get_filename_component(root_dir ${lib_dir} DIRECTORY)

  # Try to find the executable
  set(tolc_BIN_DIR ${root_dir}/bin)
  find_program(tolc_EXECUTABLE tolc PATHS ${tolc_BIN_DIR} REQUIRED)

  # Export the variables
  set(tolc_BIN_DIR
      ${tolc_BIN_DIR}
      PARENT_SCOPE)
  set(tolc_EXECUTABLE
      ${tolc_EXECUTABLE}
      PARENT_SCOPE)
endfunction()

find_tolc()
message(STATUS "Using tolc executable: ${tolc_EXECUTABLE}")

### Config variables: These determine the behaviour of tolc ###
set(tolc_pybind11_version 2.6.2)
# Comma separated list of supported languages
set(tolc_supported_languages "python")

include(${CMAKE_CURRENT_LIST_DIR}/tolcTranslate.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/GetPybind11.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tolcAddTarget.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/tolcCreateTranslation.cmake)
