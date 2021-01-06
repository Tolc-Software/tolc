include_guard()

function(find_tolc)
  # Go three level up in the package directory structure
  # Assuming something like
  #   <install>/bin/tolc
  #   <install>/lib/cmake/tolc/tolcConfig.cmake
  get_filename_component(cmake_dir ${CMAKE_CURRENT_LIST_DIR} DIRECTORY)
  get_filename_component(lib_dir ${cmake_dir} DIRECTORY)
  get_filename_component(root_dir ${lib_dir} DIRECTORY)

  # Try to find the executable
  find_program(tolc_EXECUTABLE tolc PATHS ${root_dir}/bin REQUIRED)

  # Export it
  set(tolc_EXECUTABLE
      ${tolc_EXECUTABLE}
      PARENT_SCOPE)
endfunction()

find_tolc()
message(STATUS "Using tolc executable: ${tolc_EXECUTABLE}")

include(${CMAKE_CURRENT_LIST_DIR}/tolcTranslate.cmake)