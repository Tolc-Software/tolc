include_guard()

# Downloads the Parser project either from source or as a prebuilt asset
# If BUILD_FROM_SOURCE is set to true, then an ssh key is needed
# Relies on the GithubHelpers.cmake module
# ASSET_NAME will be the name of a release asset (e.g. Parser-Linux-main.tar.gz)
# Variables exported:
#   PARSER_LLVM_VERSION - version of llvm used
#   parser_SOURCE_DIR - source directory of downloaded parser
function(get_parser)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues BUILD_FROM_SOURCE)
  set(singleValues ASSET_NAME)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})

  if(ARG_BUILD_FROM_SOURCE)
    # Download source
    FetchContent_Declare(
      Parser
      GIT_REPOSITORY git@github.com:Tolc-Software/Parser.git
      GIT_TAG main)
    FetchContent_MakeAvailable(Parser)
  else()
    # On Windows you can't link a Debug build to a Release build,
    # therefore there are two binary versions available.
    # Need to distinguish between them.
    set(windows_config "")
    if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
      set(windows_config "-${CMAKE_BUILD_TYPE}")
    endif()
    # Download binary
    FetchContent_Declare(parser_entry
      URL
      https://github.com/Tolc-Software/Parser/releases/download/main-release/Parser-${CMAKE_HOST_SYSTEM_NAME}-main${windows_config}.tar.xz)

    message(STATUS "Checking if Parser needs to be downloaded...")
    FetchContent_Populate(parser_entry)

    set(Parser_ROOT ${parser_entry_SOURCE_DIR})
    find_package(
      Parser
      REQUIRED
      CONFIG
      PATHS
      ${Parser_ROOT}
      REQUIRED)
  endif()

  # Export the variables
  set(parser_SOURCE_DIR ${parser_entry_SOURCE_DIR} PARENT_SCOPE)
  set(PARSER_LLVM_VERSION ${PARSER_LLVM_VERSION} PARENT_SCOPE)
endfunction()

function(get_parser_system_include)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues LIBCPP_ROOT_DIR PARSER_SOURCE_DIR LLVM_VERSION VARIABLE)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})

  # Defines get_system_include and format_includes
  include(${ARG_PARSER_SOURCE_DIR}/lib/cmake/Parser/IncludePathHelpers.cmake)

  # Set the include path for the system library in the variable
  # We are using the standard library shipped
  # with the downloaded llvm for include paths in the parsing
  get_system_include(
    VARIABLE
      platform_include
    LLVM_DIRECTORY
      ${ARG_LIBCPP_ROOT_DIR}
    LLVM_VERSION
      ${ARG_LLVM_VERSION})

  format_includes(VARIABLE ${ARG_VARIABLE} INCLUDES
                  ${platform_include} SYSTEM)

  set(${ARG_VARIABLE} ${${ARG_VARIABLE}} PARENT_SCOPE)
endfunction()
