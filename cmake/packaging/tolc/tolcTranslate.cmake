include_guard()

function(tolc_translate_file)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues INPUT LANGUAGE MODULE_NAME OUTPUT)
  set(multiValues INCLUDES)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Cannot assume too new CMake version
  set(function_name tolc_translate_file)
  set(usage
    "Usage: ${function_name}(MODULE_NAME myLibrary LANGUAGE python INPUT include/myLibrary.hpp OUTPUT out [INCLUDES include])"
  )

  # Helper function
  function(error_with_usage msg)
    message(FATAL_ERROR "Invalid call to ${function_name}. ${msg}\n${usage}")
  endfunction()

  # Error checks on input
  if(NOT ARG_MODULE_NAME)
    error_with_usage(
      "Missing MODULE_NAME argument. Used to call the translated library in the target language."
    )
  endif()
  if(NOT ARG_LANGUAGE)
    error_with_usage(
      "Missing LANGUAGE argument. Stores what language to translate to.")
  endif()
  if(NOT ARG_INPUT)
    error_with_usage(
      "Missing INPUT argument. Stores which file to translate from.")
  endif()
  if(NOT ARG_OUTPUT)
    error_with_usage(
      "Missing OUTPUT argument. The directory to write translation files to."
    )
  endif()
  if(NOT tolc_EXECUTABLE)
    error_with_usage(
      "The variable tolc_EXECUTABLE must be set prior to calling ${function_name}. It should contain the full path of the tolc executable."
    )
  endif()

  # Turn the include directories to valid input flags
  if(ARG_INCLUDES)
    set(includes "")
    foreach(include ${ARG_INCLUDES})
      list(APPEND includes -I ${include})
    endforeach()
  else()
    set(includes "")
  endif()

  set(command
      ${tolc_EXECUTABLE}
      ${ARG_LANGUAGE}
      --module-name
      ${ARG_MODULE_NAME}
      --input
      ${ARG_INPUT}
      --output
      ${ARG_OUTPUT}
      ${includes})
  # TODO: This should be changed when tolc can handle outputs explicitly (not just a directory, but a file)
  add_custom_target(
    tolc_translate_file_${ARG_MODULE_NAME}
    ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMAND ${command}
    BYPRODUCTS ${ARG_OUTPUT}/${ARG_MODULE_NAME}.cpp
  )
endfunction()

function(tolc_translate_target)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET LANGUAGE OUTPUT)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Cannot assume too new CMake version
  set(function_name tolc_translate_target)
  set(usage
      "Usage: ${function_name}(TARGET myLibrary LANGUAGE python OUTPUT out)"
  )

  # Helper function
  function(error_with_usage msg)
    message(FATAL_ERROR "Invalid call to ${function_name}. ${msg}\n${usage}")
  endfunction()

  # Error checks on input
  if(NOT ARG_TARGET)
    error_with_usage(
      "Missing TARGET argument. What predefined target to translate.")
  endif()
  if(NOT TARGET ${ARG_TARGET})
    error_with_usage(
      "Argument TARGET is not a previously known CMake target. Got: ${ARG_TARGET}.")
  endif()
  if(NOT tolc_EXECUTABLE)
    message(FATAL_ERROR "${function_name} called without setting tolc_EXECUTABLE. Please use this module only after calling find_package(tolc).")
  endif()
  if(NOT EXISTS ${tolc_BIN_DIR}/gather_headers.sh)
    message(FATAL_ERROR "Internal error. Dependant script not found: ${tolc_BIN_DIR}/gather_headers.sh")
  endif()

  # Get the public include directories
  get_target_property(includeDirectories ${ARG_TARGET} INTERFACE_INCLUDE_DIRECTORIES)
  string(REPLACE ";" " " shellIncludes ${includeDirectories})

  # Go through the include directories for headers and build the combined header.
  # After this command, the combined header will look like:
  #   #include </home/user/project/include/h0.hpp>
  #   #include </home/user/project/someOtherInclude/h1.hpp>
  #   ...
  # NOTE: This is meant to be in POSIX shell
  set(combinedHeader ${CMAKE_CURRENT_BINARY_DIR}/tolc_${ARG_TARGET}.hpp)
  add_custom_target(
    tolc_get_public_headers_${ARG_TARGET}
    ALL
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${tolc_BIN_DIR}/gather_headers.sh ${combinedHeader} ${includeDirectories}
    BYPRODUCTS ${combinedHeader}
  )
  # Rerun when target needs to be rebuilt
  add_dependencies(tolc_get_public_headers_${ARG_TARGET} ${ARG_TARGET})

  tolc_translate_file(
    MODULE_NAME
    ${ARG_TARGET}
    LANGUAGE
    ${ARG_LANGUAGE}
    INPUT
    ${combinedHeader}
    OUTPUT
    ${ARG_OUTPUT})
  # Rerun when regathering headers
  add_dependencies(tolc_translate_file_${ARG_TARGET} tolc_get_public_headers_${ARG_TARGET})
endfunction()
