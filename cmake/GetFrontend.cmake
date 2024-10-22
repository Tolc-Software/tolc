include_guard()

# Downloads the frontend project from source NAME has to be set (e.g.
# Frontend.py, Frontend.wasm, etc) BRANCH has to be set (e.g. v0.2.0, latest,
# etc) Variables exported: ${lowercase_arg_name}_SOURCE_DIR - e.g.
# frontend.py_SOURCE_DIR source directory of downloaded frontend.py
function(get_frontend_from_source)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues BRANCH NAME)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_NAME)
    message(
      FATAL_ERROR
        "Must provide a name. i.e. get_frontend_from_source(NAME Frontend.py BRANCH v0.2.0)"
    )
  endif()

  if(NOT ARG_BRANCH)
    message(
      FATAL_ERROR
        "Must provide a BRANCH. i.e. get_frontend_from_source(NAME Frontend.py BRANCH v0.2.0)"
    )
  endif()

  include(FetchContent)
  # Download source
  string(TOLOWER ${ARG_NAME} lowercase_arg_name)
  FetchContent_Declare(
    ${lowercase_arg_name}
    GIT_REPOSITORY https://github.com/Tolc-Software/${ARG_NAME}.git
    GIT_TAG ${ARG_BRANCH} # e.g. v0.9.0
  )

  message(STATUS "Checking if ${ARG_NAME} needs to be downloaded...")
  FetchContent_MakeAvailable(${lowercase_arg_name})

  # Export the variables
  set(${lowercase_arg_name}_SOURCE_DIR
      ${${lowercase_arg_name}_entry_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()

# Downloads the frontend project as a prebuilt asset NAME has to be set (e.g.
# Frontend.py, Frontend.wasm, etc) VERSION has to be set (e.g. v0.2.0, latest,
# etc) Variables exported: ${lowercase_arg_name}_SOURCE_DIR - e.g.
# frontend.py_SOURCE_DIR source directory of downloaded frontend.py
function(get_frontend)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues VERSION NAME)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_NAME)
    message(
      FATAL_ERROR
        "Must provide a name. i.e. get_frontend(NAME Frontend.py VERSION v0.2.0)"
    )
  endif()

  if(NOT ARG_VERSION)
    message(
      FATAL_ERROR
        "Must provide a version. i.e. get_frontend(NAME Frontend.py VERSION v0.2.0)"
    )
  endif()

  include(FetchContent)
  # On Windows you can't link a Debug build to a Release build, therefore there
  # are two binary versions available. Need to distinguish between them.
  set(windows_config "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(windows_config "-${CMAKE_BUILD_TYPE}")
  endif()

  set(binary_name ${ARG_NAME}-${CMAKE_HOST_SYSTEM_NAME}${windows_config}.tar.xz)

  # Download binary
  string(TOLOWER ${ARG_NAME} lowercase_arg_name)
  FetchContent_Declare(
    ${lowercase_arg_name}_entry
    URL https://github.com/Tolc-Software/${lowercase_arg_name}/releases/download/${ARG_VERSION}/${binary_name}
  )

  message(STATUS "Checking if ${ARG_NAME} needs to be downloaded...")
  FetchContent_Populate(${lowercase_arg_name}_entry)

  set(${ARG_NAME}_ROOT ${${lowercase_arg_name}_entry_SOURCE_DIR})
  find_package(${ARG_NAME} REQUIRED CONFIG PATHS ${${ARG_NAME}_ROOT} REQUIRED)

  # Export the variables
  set(${lowercase_arg_name}_SOURCE_DIR
      ${${lowercase_arg_name}_entry_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()

# Copies the docs under ${SRC_DIR}/share/doc/${NAME}/public to
# docs/packaging/docs/python
function(copy_frontend_docs)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues NAME SRC_DIR COPY_TO)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_SRC_DIR)
    message(
      FATAL_ERROR
        "SRC_DIR not defined. Define it as the path to the downloaded frontend."
    )
  endif()

  if(NOT ARG_NAME)
    message(
      FATAL_ERROR
        "NAME not defined. Define it as the name of the downloaded frontend, e.g. 'Frontend.py'."
    )
  endif()

  if(NOT ARG_COPY_TO)
    message(
      FATAL_ERROR
        "COPY_TO not defined. Define it as the directory where to copy the documentation files, e.g. '${CMAKE_CURRENT_LIST_DIR}/docs/packaging/docs/python'."
    )
  endif()

  # Take all the files from the public documentation
  file(GLOB imported_files ${ARG_SRC_DIR}/share/doc/${ARG_NAME}/public/*)

  # Copy them to docs/python
  file(COPY ${imported_files} DESTINATION ${ARG_COPY_TO})
endfunction()
