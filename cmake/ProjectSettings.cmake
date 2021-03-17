include_guard()

find_program(CCACHE ccache)
if(CCACHE)
  set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
endif()

# Export compile flags to compile_commands.json database. Useful for linters and
# autocompletion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_IPO
       "Enable Iterprocedural Optimization, aka Link Time Optimization (LTO)"
       OFF)

if(MSVC)
  # This allows SYSTEM includes to actually not put diagnostics on the included headers
  # This should be removed in some future version of CMake:
  # https://gitlab.kitware.com/cmake/cmake/-/commit/bb61c2d024d508e623813423de3404c63a2232b9#89d801773a4603a18572684211c4c26c1a069d06
  set(CMAKE_INCLUDE_SYSTEM_FLAG_CXX "-imsvc " CACHE STRING "This is used before SYSTEM includes." FORCE)

  # Set /MT (or /MTd if debug)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>" CACHE STRING "Used to tell which runtime lib to link with." FORCE)
endif()

if(ENABLE_IPO)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)
  if(result)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
  else()
    message(SEND_ERROR "IPO is not supported: ${output}")
  endif()
endif()
