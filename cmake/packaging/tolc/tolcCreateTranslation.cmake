include_guard()

function(tolc_create_translation)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET LANGUAGE OUTPUT MODULE_NAME)
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
      "Missing TARGET argument. The name of the library to base the translation off.")
  endif()
  if(NOT ARG_LANGUAGE)
    error_with_usage(
      "Missing LANGUAGE argument. The language module to download. E.g. for 'python', this would automatically download pybind11.")
  endif()
  if(NOT ARG_INPUT)
    error_with_usage(
      "Missing INPUT argument. The source file(s) to be compiled.")
  endif()
  set(moduleName ${ARG_MODULE_NAME})
  if(NOT ARG_MODULE_NAME)
    set(moduleName ${ARG_TARGET})
  endif()

  tolc_translate_target(TARGET ${ARG_TARGET} LANGUAGE ${ARG_LANGUAGE} OUTPUT ${ARG_OUTPUT})
endfunction()
