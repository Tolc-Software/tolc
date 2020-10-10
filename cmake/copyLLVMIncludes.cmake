include_guard()

function(copy_llvm_includes)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues LLVM_DIR LLVM_VERSION OUTPUT_DIR)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET_DIR
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_LLVM_DIR)
    message(FATAL_ERROR "No LLVM directory specified.")
  endif()
  if(NOT ARG_LLVM_VERSION)
    message(FATAL_ERROR "No LLVM version specified.")
  endif()
  if(NOT ARG_OUTPUT_DIR)
    message(
      FATAL_ERROR
        "No output directory specified. NOTE: Has to be absolute, or will be relative to whatever the current directory happens to be."
    )
  endif()

  foreach(includeDir ${ARG_LLVM_DIR}/include
                     ${ARG_LLVM_DIR}/lib/clang/${ARG_LLVM_VERSION}/include)
    # Get the directory path that should hold the include files
    string(REPLACE "${ARG_LLVM_DIR}" "" relativeDir ${includeDir})
    message(STATUS ${includeDir})
    message(STATUS ${relativeDir})
    execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory
                            ${ARG_OUTPUT_DIR}/${relativeDir})
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${includeDir}
                            ${ARG_OUTPUT_DIR}/${relativeDir})
  endforeach()
endfunction()
