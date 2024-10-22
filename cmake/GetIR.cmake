include_guard()

# Downloads the IR project as an asset
# VERSION has to be set (i.e. v0.2.0, latest, etc)
# Variables exported:
#   ir_SOURCE_DIR - source directory of downloaded parser
function(get_ir)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues VERSION)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_VERSION)
    message(FATAL_ERROR "Must provide a version. i.e. get_ir(VERSION v0.11.0)")
  endif()

  include(FetchContent)
  # Download binary
  FetchContent_Declare(
    ir
    URL https://github.com/Tolc-Software/IntermediateRepresentation/releases/download/${ARG_VERSION}/IR.tar.xz
  )

  message(STATUS "Checking if Parser needs to be downloaded...")
  FetchContent_Populate(ir)

  set(IR_ROOT ${ir_SOURCE_DIR})
  find_package(IR REQUIRED CONFIG PATHS ${IR_ROOT} REQUIRED)

  # Export the variables
  set(ir_SOURCE_DIR
      ${ir_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()
