include_guard()

function(add_options)
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

  target_compile_features(${ARG_TARGET} PRIVATE cxx_std_17)
  if(NOT MSVC)
    # Create Position Independent Executables when building this lib/executable
    # NOTE: Also needed for static libraries
    # https://mropert.github.io/2018/02/02/pic_pie_sanitizers/
    target_compile_options(${ARG_TARGET} PRIVATE -fPIE)
  endif()
endfunction()

function(add_warnings)
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

  # Set warning flags for different compilers
  if(MSVC)
    target_compile_options(
      ${ARG_TARGET}
      PRIVATE /W4
                /WX # Treat warnings as errors
                "/permissive-" # Adhere to standard
                /w14242 # 'identfier': conversion from 'type1' to 'type1',
                        # possible loss of data
                /w14254 # 'Operator': conversion from 'type1:field_bits' to
                        # 'type2:field_bits', possible loss of data
                /w14263 # 'Function': member function does not override any base
                        # class virtual member function
                /w14265 # 'Classname': class has virtual functions, but destructor
                        # is not virtual instances of this class may not be
                        # destructed correctly
                /w14287 # 'Operator': unsigned/negative constant mismatch
                /we4289 # Nonstandard extension used: 'variable': loop control
                        # variable declared in the for-loop is used outside the
                        # for-loop scope
                /w14296 # 'Operator': expression is always 'boolean_value'
                /w14311 # 'Variable': pointer truncation from 'type1' to 'type2'
                /w14545 # Expression before comma evaluates to a function which is
                        # missing an argument list
                /w14546 # Function call before comma missing argument list
                /w14547 # 'Operator': operator before comma has no effect;
                        # expected operator with side-effect
                /w14549 # 'Operator': operator before comma has no effect; did you
                        # intend 'operator'?
                /w14555 # Expression has no effect; expected expression with side-
                        # effect
                /w14619 # Pragma warning: there is no warning number 'number'
                /w14640 # Enable warning on thread un-safe static member
                        # initialization
                /w14826 # Conversion from 'type1' to 'type_2' is sign-extended.
                        # This may cause unexpected runtime behavior.
                /w14905 # Wide string literal cast to 'LPSTR'
                /w14906 # String literal cast to 'LPWSTR'
                /w14928 # Illegal copy-initialization; more than one user-defined
                        # conversion has been implicitly applied
    )
  else()
    # Either gcc or Clang

    target_compile_options(
      ${ARG_TARGET}
      PRIVATE -Wall
                -Werror # treat all warnings as errors
                -Wextra # reasonable and standard
                -Wshadow # warn the user if a variable declaration shadows one
                         # from a parent context
                -Wnon-virtual-dtor # warn the user if a class with virtual
                                   # functions has a non-virtual destructor. This
                                   # helps catch hard to track down memory errors
                -Wold-style-cast # warn for c-style casts
                -Wcast-align # warn for potential performance problem casts
                -Wunused # warn on anything being unused
                -Woverloaded-virtual # warn if you overload (not override) a
                                     # virtual function
                -Wpedantic # warn if non-standard C++ is used
                -Wconversion # warn on type conversions that may lose data
                -Wsign-conversion # warn on sign conversions
                -Wnull-dereference # warn if a null dereference is detected
                -Wdouble-promotion # warn if float is implicit promoted to double
                -Wdate-time # warn on use of time macros
                -Wformat=2 # warn on security issues around functions that format
                           # output (ie printf)
    )

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
      target_compile_options(
        ${ARG_TARGET}
        PRIVATE -Wmisleading-indentation # warn if identation implies blocks
                                           # where blocks do not exist
                  -Wduplicated-cond # warn if if / else chain has duplicated
                                    # conditions
                  -Wduplicated-branches # warn if if / else branches have
                                        # duplicated code
                  -Wlogical-op # warn about logical operations being used where
                               # bitwise were probably wanted
                  -Wuseless-cast # warn if you perform a cast to the same type
      )
    elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
      target_compile_options(
        ${ARG_TARGET}
        PRIVATE -Wduplicate-enum
                  -fdiagnostics-absolute-paths # To avoid confusion about which
                                               # file is built
      )
    endif()
  endif()
endfunction()
