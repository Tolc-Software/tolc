include_guard()

function(tolc_create_bindings)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues DO_NOT_SEARCH_TARGET_INCLUDES NO_ANALYTICS)
  set(singleValues TARGET LANGUAGE OUTPUT)
  set(multiValues HEADERS)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  # Variables related to error messages:
  # Cannot assume too new CMake version
  set(function_name tolc_create_bindings)
  set(usage "Usage: ${function_name}(TARGET myLibrary LANGUAGE python)")

  # Helper function
  function(error_with_usage msg)
    message(FATAL_ERROR "Invalid call to ${function_name}. ${msg}\n${usage}")
  endfunction()

  # Error checks on input
  if(NOT ARG_TARGET)
    error_with_usage(
      "Missing TARGET argument. The name of the library to base the translation off."
    )
  endif()
  if(NOT ARG_LANGUAGE)
    error_with_usage(
      "Missing LANGUAGE argument. The language module to download. E.g. for 'python', this would automatically download pybind11."
    )
  endif()

  # Make sure the headers flag gets propadated correctly
  set(headers "")
  if(ARG_HEADERS)
    set(headers HEADERS ${ARG_HEADERS})
  endif()
  set(doNotSearchTargetIncludes "")
  if(ARG_DO_NOT_SEARCH_TARGET_INCLUDES)
    set(doNotSearchTargetIncludes DO_NOT_SEARCH_TARGET_INCLUDES)
  endif()
  set(noAnalytics "")
  if(ARG_NO_ANALYTICS)
    set(noAnalytics NO_ANALYTICS)
  endif()

  # What the actual target name will be
  set(tolc_target_name ${ARG_TARGET}_${ARG_LANGUAGE})
  message(
    STATUS "Creating bindings to ${ARG_LANGUAGE} in target ${tolc_target_name}")

  # Use the input target to create the translation
  tolc_translate_target(
    TARGET
    ${ARG_TARGET}
    ${doNotSearchTargetIncludes}
    ${noAnalytics}
    ${headers}
    LANGUAGE
    ${ARG_LANGUAGE}
    OUTPUT
    ${ARG_OUTPUT})

  if(${ARG_LANGUAGE} MATCHES "python")
    # NOTE: Variable injected from tolcConfig file
    get_pybind11(VERSION ${tolc_pybind11_version})
    # Create the python module
    pybind11_add_module(${tolc_target_name}
                        ${ARG_OUTPUT}/${ARG_TARGET}_python.cpp)
  elseif(${ARG_LANGUAGE} MATCHES "wasm")
    # Assumes that the Emscripten toolchain file is used
    # Will result in a .js and a .wasm file
    add_executable(${tolc_target_name} ${ARG_OUTPUT}/${ARG_TARGET}_wasm.cpp)

    # Export Promise as 'loadMyLib' for module 'MyLib'
    # -s MODULARIZE=1 sets it as a promise based load
    # Note that this is necessary for preJS to work properly
    set_target_properties(
      ${tolc_target_name}
      PROPERTIES
        LINK_FLAGS
        "-s MODULARIZE=1 -s EXPORT_NAME=\"load${ARG_TARGET}\" --pre-js ${ARG_OUTPUT}/pre.js -lembind"
    )
  else()
    error_with_usage(
      "Unknown language input: ${ARG_LANGUAGE}. Valid input: [${tolc_supported_languages}]"
    )
  endif()

  # The added library target depends on the target being translated
  add_dependencies(${tolc_target_name} tolc_translate_file_${ARG_TARGET})

  # NOTE: The user may need to provide additional links if they have their PUBLIC/PRIVATE dependencies missmatched
  target_link_libraries(${tolc_target_name} PRIVATE ${ARG_TARGET})

  set_target_properties(
    ${tolc_target_name}
    PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tolc"
               LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tolc"
               RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/tolc")
  # This allows the target to be called target_language, but still be imported e.g. in python as 'import target'
  set_target_properties(${tolc_target_name} PROPERTIES OUTPUT_NAME
                                                       ${ARG_TARGET})
endfunction()
