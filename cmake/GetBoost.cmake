include_guard()

# Downloads the boost project either from 
# VERSION - version of boost used
# Variables exported:
#   boost_SOURCE_DIR
function(get_boost)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues VERSION)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})

  if(NOT ARG_VERSION)
    message(FATAL_ERROR "Called get_boost without a VERSION argument.")
  endif()

  include(FetchContent)
  string(REPLACE "." "_" underscore_version ${ARG_VERSION})
  FetchContent_Declare(boost
                       URL https://dl.bintray.com/boostorg/release/${ARG_VERSION}/source/boost_${underscore_version}.tar.gz)

  if(NOT boost_POPULATED)
    FetchContent_Populate(boost)
  endif()
  # Boost include directory should now be available as boost_SOURCE_DIR
  # Export the variables
  set(boost_SOURCE_DIR
      ${boost_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()

# Links input TARGET to the boost downloaded from get_boost
# Expects boost_SOURCE_DIR to be available
function(target_link_boost)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})

  if(NOT ARG_TARGET)
    message(FATAL_ERROR "Called target_link_boost without a TARGET argument.")
  endif()

  if(NOT boost_SOURCE_DIR)
    message(FATAL_ERROR "Called target_link_boost without boost_SOURCE_DIR having a VALUE. Did you call get_boost before this function.")
  endif()

  # Make boost headers avalable
  target_include_directories(${ARG_TARGET} PRIVATE ${boost_SOURCE_DIR})

  # Do not try to link against static libraries (only using headers for now)
  target_compile_definitions(${ARG_TARGET} PRIVATE BOOST_ALL_NO_LIB)
endfunction()
