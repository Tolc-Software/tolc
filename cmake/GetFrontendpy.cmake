include_guard()

# Downloads the frontend.py project either from source or as a prebuilt asset
# The environment variable GITHUB_AUTH_TOKEN will be read and ASSET_NAME is required
# Relies on the GithubHelpers.cmake module
# GITHUB_AUTH_TOKEN is a token generated by github to query things about github
# ASSET_NAME will be the name of a release asset (e.g. Frontend.py-Linux-master.tar.gz)
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

  # Try to download the prebuilt version
  if(NOT DEFINED ENV{GITHUB_AUTH_TOKEN})
    message(
      FATAL_ERROR
        "GITHUB_AUTH_TOKEN not defined as a environment variable. Cannot download assets from Github."
    )
  endif()
  include(${PROJECT_SOURCE_DIR}/cmake/GithubHelpers.cmake)
  fetch_asset_from_github(
    FETCH_VARIABLE
    frontend_py
    GITHUB_TOKEN
    $ENV{GITHUB_AUTH_TOKEN}
    USER
    srydell
    REPOSITORY
    frontend.py
    TAG
    master-release
    ASSET_NAME
    ${ARG_ASSET_NAME})

  set(Frontend.py_ROOT ${frontend_py_SOURCE_DIR})
  find_package(Frontend.py REQUIRED CONFIG PATHS ${Frontend.py_ROOT} REQUIRED)

  # Export the variables
  set(frontend_py_SOURCE_DIR
      ${frontend_py_SOURCE_DIR}
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
  file(
    GLOB
      imported_files
      ${ARG_SRC_DIR}/share/doc/Frontend.py/public/*)

  # Copy them to docs/python
  file(
    COPY
      ${imported_files}
    DESTINATION
      ${PROJECT_SOURCE_DIR}/docs/packaging/docs/python)
endfunction()
