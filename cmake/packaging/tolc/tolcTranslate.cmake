include_guard()

function(tolc_translate_file)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues FILE LANGUAGE MODULE_NAME OUTPUT_DIR)
  set(multiValues INCLUDES)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Cannot assume too new CMake version
  set(function_name tolc_translate_file)
  set(usage
      "Usage: ${function_name}(MODULE_NAME myLibrary LANGUAGE python FILE include/myLibrary.hpp OUTPUT_DIR out [INCLUDES include])"
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
  if(NOT ARG_FILE)
    error_with_usage(
      "Missing FILE argument. Stores which file to translate from.")
  endif()
  if(NOT ARG_OUTPUT_DIR)
    error_with_usage(
      "Missing OUTPUT_DIR argument. The directory to write translation files to."
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
      ${ARG_FILE}
      --output
      ${ARG_OUTPUT_DIR}
      ${includes})
  execute_process(
    COMMAND ${command}
    RESULT_VARIABLE errorCode
    OUTPUT_VARIABLE errorMessage)

  if(NOT errorCode EQUAL 0)
    # Convert to string
    string(REPLACE ";" " " commandString "${command}")
    message(WARNING "Failed running the following command: ${commandString}")
    message(
      FATAL_ERROR
        "Tolc failed to run with the following error message:\n${errorMessage}")
  endif()
endfunction()

function(tolc_translate_target)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET LANGUAGE OUTPUT_DIR)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Cannot assume too new CMake version
  set(function_name tolc_translate_target)
  set(usage
      "Usage: ${function_name}(TARGET myLibrary LANGUAGE python OUTPUT_DIR out)"
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

  # Get the public include directories
  get_target_property(includeDirectories ${ARG_TARGET} INCLUDE_DIRECTORIES)
  # Go through the includes and find the public headers
  set(headersToCombine "")
  if(includeDirectories)
    foreach(includeDir ${includeDirectories})
      # NOTE: This is done at configure time
      file(GLOB_RECURSE headers "${includeDir}/*.hpp" "${includeDir}/*.h")
      list(APPEND headersToCombine ${headers})
    endforeach()
  endif()

  if(NOT headersToCombine)
    error_with_usage(
      "Did not find any headers to translate in the public include directories (searched ${includeDirectories}) for target ${ARG_TARGET}")
  endif()
  list(REMOVE_DUPLICATES headersToCombine)

  # Combine to a parseable file
  set(combinedHeadersString "")
  foreach(header ${headersToCombine})
    string(APPEND combinedHeadersString "#include \"${header}\"\n")
  endforeach()

  # NOTE: This is done at configure time
  set(combinedHeader ${CMAKE_CURRENT_BINARY_DIR}/tolc_${ARG_TARGET}_public_headers.hpp)
  file(WRITE
    ${combinedHeader}
    ${combinedHeadersString})

  tolc_translate_file(
    MODULE_NAME
    ${ARG_TARGET}
    LANGUAGE
    ${ARG_LANGUAGE}
    FILE
    ${combinedHeader}
    OUTPUT_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/out)
endfunction()
