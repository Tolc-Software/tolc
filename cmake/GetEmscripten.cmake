include_guard()

function(get_emscripten)
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
      FATAL_ERROR "Must provide a version. e.g. getEmscripten(VERSION 3.1.3)")
  endif()

  # Download the SDK
  set(emsdk_version ${ARG_VERSION})
  include(FetchContent)
  FetchContent_Declare(
    emsdk_entry
    URL https://github.com/emscripten-core/emsdk/archive/refs/tags/${emsdk_version}.tar.gz
  )

  FetchContent_GetProperties(emsdk_entry)
  if(NOT emscripten_entry_POPULATED)
    FetchContent_Populate(emsdk_entry)
  endif()

  set(sdkCommand ./emsdk)
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(sdkCommand emsdk.bat)
  endif()

  # Installs the Emscripten compiler
  execute_process(COMMAND ${sdkCommand} install ${emsdk_version}
                  WORKING_DIRECTORY ${emsdk_entry_SOURCE_DIR})

  # Writes the .emscripten file
  execute_process(COMMAND ${sdkCommand} activate ${emsdk_version}
                  WORKING_DIRECTORY ${emsdk_entry_SOURCE_DIR})

  # Export the variables
  set(emsdk_SOURCE_DIR
      ${emsdk_entry_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()
