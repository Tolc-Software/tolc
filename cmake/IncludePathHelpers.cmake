include_guard()

# Populate the input variable with a string/list with the platform specific
# includes.
# Variables:
#   LLVM_DIRECTORY: Directory of downloaded LLVM
#   LLVM_VERSION: Version of downloaded LLVM
#   VARIABLE: Variable name to store the include paths
# Example: get_system_include(LLVM_DIRECTORY ${my_llvm_path} LLVM_VERSION 10.0.0 VARIABLE my_includes)
# message(STATUS "My includes are: ${my_includes}")
function(get_system_include)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues LLVM_DIRECTORY LLVM_VERSION VARIABLE)
  set(multiValues)
  # Process the arguments passed in
  # can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix}
                        "${noValues}"
                        "${singleValues}"
                        "${multiValues}"
                        ${ARGN})
  if(NOT ARG_LLVM_DIRECTORY OR NOT ARG_LLVM_VERSION)
    message(FATAL_ERROR "Not enough information about LLVM")
  endif()
  if(NOT ARG_VARIABLE)
    message(FATAL_ERROR "VARIABLE must be set.")
  endif()
  string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)"
    LLVM_FULL_MATCH ${ARG_LLVM_VERSION})
  set(ARG_LLVM_VERSION_MAJOR ${CMAKE_MATCH_1})
  set(ARG_LLVM_VERSION_MINOR ${CMAKE_MATCH_2})
  set(ARG_LLVM_VERSION_PATCH ${CMAKE_MATCH_3})
  # We are using the standard library shipped with the downloaded llvm (libc++) for include paths in the parsing
  # But we also need some platform specific code which cannot be assumed
  # You can find the include paths by typing: $ /path/to/llvm/imported/clang -v -x c++ -
  # These correspond to the way they appear in the command above (the order is strict because they use #include_next)
  set(system_include "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    set(system_include
        "${ARG_LLVM_DIRECTORY}/include/c++/v1"
        "/usr/local/include"
        "${ARG_LLVM_DIRECTORY}/lib/clang/${ARG_LLVM_VERSION}/include"
        "${ARG_LLVM_DIRECTORY}/lib/clang/${ARG_LLVM_VERSION_MAJOR}/include"
        "/usr/include"
 "${ARG_LLVM_DIRECTORY}/include/x86_64-unknown-linux-gnu/c++/v1/")
  elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Darwin)
    set(system_include
         "${ARG_LLVM_DIRECTORY}/include/c++/v1"
         "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include"
         "${ARG_LLVM_DIRECTORY}/lib/clang/${ARG_LLVM_VERSION}/include"
         "${ARG_LLVM_DIRECTORY}/lib/clang/${ARG_LLVM_VERSION_MAJOR}/include"
         "/usr/local/include"
         "/System/Library/Frameworks"
         "/Library/Frameworks")
  elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    set(version_placeholder "{LATEST_VERSION}")
    set(version_placeholder
        ${version_placeholder}
        PARENT_SCOPE)
    set(system_include
        "${ARG_LLVM_DIRECTORY}/lib/clang/${ARG_LLVM_VERSION}/include"
        "${ARG_LLVM_DIRECTORY}/lib/clang/${ARG_LLVM_VERSION_MAJOR}/include"
        "C:/Program Files (x86)/Microsoft Visual Studio/${version_placeholder}/Enterprise/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/Program Files (x86)/Microsoft Visual Studio/${version_placeholder}/Professional/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/Program Files (x86)/Microsoft Visual Studio/${version_placeholder}/Community/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/Program Files/Microsoft Visual Studio/${version_placeholder}/Enterprise/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/Program Files/Microsoft Visual Studio/${version_placeholder}/Professional/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/Program Files/Microsoft Visual Studio/${version_placeholder}/Community/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/BuildTools/VC/Tools/MSVC/${version_placeholder}/include"
        "C:/Program Files (x86)/Windows Kits/${version_placeholder}/include/${version_placeholder}/ucrt"
        "C:/Program Files (x86)/Windows Kits/${version_placeholder}/include/${version_placeholder}/shared"
        "C:/Program Files (x86)/Windows Kits/${version_placeholder}/include/${version_placeholder}/um"
        "C:/Program Files (x86)/Windows Kits/${version_placeholder}/include/${version_placeholder}/winrt"
        "C:/Program Files (x86)/Windows Kits/${version_placeholder}/include/${version_placeholder}/cppwinrt")
  else()
    message(FATAL_ERROR "Unsupported platform for now.")
  endif()
  set(${ARG_VARIABLE}
      ${system_include}
      PARENT_SCOPE)
endfunction()

# Format the INCLUDES Example: format_includes(VARIABLE my_formated_includes
# INCLUDES my_includes) message(STATUS "${my_includes}") ---
# /usr/include;/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include
# message(STATUS "${my_formated_includes}") --- "-I/usr/include",
# "-I/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr/include" NOTE: If
# you want system includes (-isystem) use the SYSTEM flag
function(format_includes)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues SYSTEM)
  set(singleValues VARIABLE)
  set(multiValues INCLUDES)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_VARIABLE)
    message(FATAL_ERROR "VARIABLE must be set.")
  endif()

  set(include_flag "-I")
  if(ARG_SYSTEM)
    set(include_flag "-isystem")
  endif()

  set(temp "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    # Use the windowsPathExtraction.hpp
    foreach(include ${ARG_INCLUDES})
      if(NOT temp)
        set(temp "Parser::Windows::appendSystemIncludes(Parser::Windows::filterPathsWithLatestVersion({\"${include}\"")
      else()
        string(APPEND temp ", \"${include}\"")
      endif()
    endforeach()
    if(version_placeholder)
      string(APPEND temp "}, \"${version_placeholder}\"))")
    else()
      message(FATAL_ERROR "version_placeholder must be set. Was it not set from the call to get_system_include?")
    endif()
  else()
    # format as: "-Iinclude[0]", "-Iinclude[1]", ...
    foreach(include ${ARG_INCLUDES})
      if(NOT temp)
        set(temp "{\"${include_flag}${include}\"")
      else()
        string(APPEND temp ", \"${include_flag}${include}\"")
      endif()
    endforeach()
    string(APPEND temp "}")
  endif()
  set(${ARG_VARIABLE}
      ${temp}
      PARENT_SCOPE)
endfunction()

# Tests: format_includes(VARIABLE bla INCLUDES "Hi;Ho") message(STATUS ${bla})

# get_platform_specific_includes(VARIABLE includes) message(STATUS
# "${includes}") format_includes(VARIABLE bla INCLUDES ${includes})
# message(STATUS ${bla}) format_includes(VARIABLE bla INCLUDES ${includes}
# SYSTEM) message(STATUS ${bla})
