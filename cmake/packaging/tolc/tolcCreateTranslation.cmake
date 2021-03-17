include_guard()

function(tolc_create_translation)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET LANGUAGE OUTPUT)
  set(multiValues INCLUDES)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Variables related to error messages:
  # Cannot assume too new CMake version
  set(function_name tolc_create_translation)
  set(usage
    "Usage: ${function_name}(TARGET myLibrary LANGUAGE python)"
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
  set(moduleName ${ARG_MODULE_NAME})
  if(NOT ARG_MODULE_NAME)
    set(moduleName ${ARG_TARGET})
  endif()

  # What the actual target name will be
  set(tolcTargetName ${ARG_TARGET}_${ARG_LANGUAGE})
  message(STATUS "Creating translation to language ${ARG_LANGUAGE} in target ${tolcTargetName}")

  # Use the input target to create the translation
  tolc_translate_target(TARGET ${ARG_TARGET} LANGUAGE ${ARG_LANGUAGE} OUTPUT ${ARG_OUTPUT})
  # Add a new target, representing the translation
  # TODO: This should be changed when tolc can handle outputs explicitly (not just a directory, but a file)
  tolc_add_library(TARGET ${tolcTargetName} LANGUAGE ${ARG_LANGUAGE} INPUT ${ARG_OUTPUT}/${ARG_TARGET}.cpp)

  # The added library target depends on the target being translated
  add_dependencies(${tolcTargetName} tolc_translate_file_${ARG_TARGET})

  # NOTE: The user may need to provide additional links if they have their PUBLIC/PRIVATE dependencies missmatched
  target_link_libraries(${tolcTargetName} PRIVATE ${ARG_TARGET})

  set_target_properties(${tolcTargetName}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tolc"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tolc"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tolc"
  )
  # This allows the target to be called target_language, but still be imported e.g. in python as 'import target'
  set_target_properties(${tolcTargetName} PROPERTIES OUTPUT_NAME ${ARG_TARGET})
endfunction()
