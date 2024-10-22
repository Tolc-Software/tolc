include_guard()

function(_get_llvm_download_url)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues VERSION VARIABLE)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_TARGET
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  set(download_url "")
  if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Linux)
    set(download_url
        https://github.com/llvm/llvm-project/releases/download/llvmorg-${ARG_VERSION}/clang+llvm-${ARG_VERSION}-x86_64-linux-gnu-ubuntu-18.04.tar.xz
    )
  elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Darwin)
    execute_process(COMMAND uname -m OUTPUT_VARIABLE architecture)
    string(STRIP ${architecture} architecture)

    if(${architecture} STREQUAL arm64)
      set(download_url
          https://github.com/llvm/llvm-project/releases/download/llvmorg-${ARG_VERSION}/LLVM-${ARG_VERSION}-macOS-ARM64.tar.xz
      )
    else()
      set(download_url
          https://github.com/llvm/llvm-project/releases/download/llvmorg-${ARG_VERSION}/LLVM-${ARG_VERSION}-macOS-X64.tar.xz
      )
    endif()
  elseif(${CMAKE_HOST_SYSTEM_NAME} STREQUAL Windows)
    if(${CMAKE_BUILD_TYPE} STREQUAL Debug)
      set(download_url
          https://github.com/Tolc-Software/custom-llvm/releases/download/${ARG_VERSION}/clang+llvm-${ARG_VERSION}-x86_64-windows-Debug.tar.xz
      )
    else()
      set(download_url
          https://github.com/Tolc-Software/custom-llvm/releases/download/${ARG_VERSION}/clang+llvm-${ARG_VERSION}-x86_64-windows-Release.tar.xz
      )
    endif()
  else()
    message(FATAL_ERROR "Unsupported platform for now: ${CMAKE_HOST_SYSTEM}")
  endif()
  set(${ARG_VARIABLE}
      ${download_url}
      PARENT_SCOPE)
endfunction()

function(setup_llvm)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues NAME VERSION)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_NAME
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_NAME)
    message(
      FATAL_ERROR
        "Must provide a name. e.g. setupLLVM(NAME LLVM_ENTRY VERSION 9.0.0)")
  endif()
  if(NOT ARG_VERSION)
    message(
      FATAL_ERROR
        "Must provide a version. e.g. setupLLVM(NAME LLVM_ENTRY VERSION 9.0.0)")
  endif()

  # All fetchcontent stuff uses lowercase names
  string(TOLOWER "${ARG_NAME}" llvm_entry)

  include(FetchContent)

  _get_llvm_download_url(VERSION ${ARG_VERSION} VARIABLE DOWNLOAD_URL)

  FetchContent_Declare(${llvm_entry} URL ${DOWNLOAD_URL})

  message(STATUS "Checking if LLVM needs to be downloaded...")
  FetchContent_Populate(${llvm_entry})
  message(STATUS "LLVM source dir: ${${llvm_entry}_SOURCE_DIR}")
  message(STATUS "LLVM include dir: ${${llvm_entry}_SOURCE_DIR}/include")

  # Export the variables
  set(${llvm_entry}_INCLUDE_DIR
      ${${llvm_entry}_SOURCE_DIR}/include
      PARENT_SCOPE)
  set(${llvm_entry}_SOURCE_DIR
      ${${llvm_entry}_SOURCE_DIR}
      PARENT_SCOPE)
endfunction()

macro(get_clang_and_llvm)
  # Define the supported set of keywords
  set(prefix ARG)
  set(noValues)
  set(singleValues PATH)
  set(multiValues)
  # Process the arguments passed in can be used e.g. via ARG_PATH
  cmake_parse_arguments(${prefix} "${noValues}" "${singleValues}"
                        "${multiValues}" ${ARGN})

  if(NOT ARG_PATH)
    message(
      FATAL_ERROR
        "Must provide a path to the clang installation. e.g. getClang(PATH $\{llvm_entry_SOURCE_DIR\})"
    )
  endif()

  set(Clang_ROOT ${ARG_PATH})
  find_package(
    Clang
    REQUIRED
    CONFIG
    PATHS
    ${Clang_ROOT}
    REQUIRED
    NO_DEFAULT_PATH)

  set(LLVM_ROOT ${ARG_PATH})
  find_package(
    LLVM
    REQUIRED
    CONFIG
    PATHS
    ${LLVM_ROOT}
    REQUIRED
    NO_DEFAULT_PATH)
  list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")
endmacro()
