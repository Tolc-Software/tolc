cmake_minimum_required(VERSION 3.17)

project(
  tolc
  VERSION 0.1
  LANGUAGES CXX)

# Define project_warnings and project_options targets
include(cmake/WarningsAndOptions.cmake)
add_warnings(TARGET tolc_warnings)
add_options(TARGET tolc_options)

# Define options such as ccache
include(cmake/ProjectSettings.cmake)

include(cmake/Sanitizers.cmake)

include(cmake/StaticAnalyzers.cmake)

include(cmake/CodeCoverage.cmake)

add_executable(tolc src/tolc.main.cpp)
target_link_libraries(tolc PRIVATE tolc_warnings tolc_options)
set_target_properties(tolc PROPERTIES CXX_STANDARD_REQUIRED ON CXX_EXTENSIONS
                                                               OFF)

# Only build tests and benchmarks if building this project directly
if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
  option(ENABLE_TESTING "Build the testing tree" OFF)
  if(ENABLE_TESTING)
    # TODO: Add tests
    # enable_testing()
    # add_subdirectory(tests)
  endif()

  option(ENABLE_BENCHMARKS "Build the benchmarks" OFF)
  if(ENABLE_BENCHMARKS)
    # TODO: Add benchmarks
  endif()
endif()
