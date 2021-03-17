include_guard()

function(get_pybind11)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues VERSION)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_NAME
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_VERSION)
    message(
      FATAL_ERROR "Must provide a version. e.g. getPybind11(VERSION 2.6.2)")
  endif()

  # All fetchcontent stuff uses lowercase names
  string(TOLOWER "${ARG_NAME}" pybind11_entry)

  include(FetchContent)

  FetchContent_Declare(
    pybind11_entry
    URL https://github.com/pybind/pybind11/archive/v${ARG_VERSION}.tar.gz)

  # Includes the necessary directories
  FetchContent_MakeAvailable(pybind11_entry)
endfunction()
