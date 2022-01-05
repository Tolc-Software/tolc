include_guard()

# Downloads the frontend.py project either from source or as a prebuilt asset
# ASSET_NAME will be the name of a release asset (e.g. Frontend.py-Linux-main.tar.gz)
# Variables exported:
#   frontend.py_SOURCE_DIR - source directory of downloaded frontend.py
function(get_frontend_py)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues ASSET_NAME)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  include(FetchContent)
  # On Windows you can't link a Debug build to a Release build,
  # therefore there are two binary versions available.
  # Need to distinguish between them.
  set(windows_config "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(windows_config "-${CMAKE_BUILD_TYPE}")
  endif()
  # Download binary
  FetchContent_Declare(
    frontend_py_entry
    URL https://github.com/Tolc-Software/frontend.py/releases/download/main-release/frontend.py-${CMAKE_HOST_SYSTEM_NAME}-main${windows_config}.tar.xz
  )

  message(STATUS "Checking if frontend.py needs to be downloaded...")
  FetchContent_Populate(frontend_py_entry)

  set(Frontend.py_ROOT ${frontend_py_entry_SOURCE_DIR})
  find_package(Frontend.py REQUIRED CONFIG PATHS ${Frontend.py_ROOT} REQUIRED)

  # Export the variables
  set(frontend_py_SOURCE_DIR
      ${frontend_py_entry_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()

# Copies the docs under {SRC_DIR}/share/doc/Frontend.py/public to docs/packaging/docs/python
function(copy_frontend_py_docs)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues SRC_DIR)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_SRC_DIR)
    message(
      FATAL_ERROR
        "SRC_DIR not defined. Define it as the path to the downloaded frontend.py."
    )
  endif()

  # Take all the files from the public documentation
  file(GLOB imported_files ${ARG_SRC_DIR}/share/doc/Frontend.py/public/*)

  # Copy them to docs/python
  file(COPY ${imported_files}
       DESTINATION ${PROJECT_SOURCE_DIR}/docs/packaging/docs/python)
endfunction()
