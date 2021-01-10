include_guard()

function(tolc_create_translation)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET LANGUAGE OUTPUT_DIR)
  set(multiValues INCLUDES INPUT)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Variables related to error messages:
  # Cannot assume too new CMake version
  set(function_name tolc_add_library)
  set(usage
    "Usage: ${function_name}(TARGET myLibrary LANGUAGE python INPUT src/tolcGeneratedFile.cpp src/myOwnExtension.cpp)"
  )

  # Helper function
  function(error_with_usage msg)
    message(FATAL_ERROR "Invalid call to ${function_name}. ${msg}\n${usage}")
  endfunction()

  # Error checks on input
  if(NOT ARG_TARGET)
    error_with_usage(
      "Missing TARGET argument. The name of the library.")
  endif()
  if(NOT ARG_LANGUAGE)
    error_with_usage(
      "Missing LANGUAGE argument. The language module to download. E.g. for 'python', this would automatically download pybind11.")
  endif()
  if(NOT ARG_INPUT)
    error_with_usage(
      "Missing INPUT argument. The source file(s) to be compiled.")
  endif()

  if(ARG_LANGUAGE EQUAL "python")
    # NOTE: Variable injected from tolcConfig file
    get_pybind11(VERSION ${tolc_pybind11_version})
    # Create the python module
    pybind11_add_module(${ARG_TARGET} ${ARG_INPUT} SYSTEM)
  else()
    error_with_usage(
      "Unknown language input: ${ARG_LANGUAGE}. Valid input: [${tolc_supported_languages}]")
  endif()
endfunction()
