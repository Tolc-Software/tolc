include_guard()

function(tolc_add_options)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues TARGET)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  target_compile_features(${ARG_TARGET} PRIVATE cxx_std_17)
  if(NOT MSVC)
    # Create Position Independent Executables when building this lib/executable
    # NOTE: Also needed for static libraries
    # https://mropert.github.io/2018/02/02/pic_pie_sanitizers/
    target_compile_options(${ARG_TARGET} PRIVATE -fPIE)
  endif()
endfunction()
