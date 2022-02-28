include_guard()

# NOTE: This fixture is used for all packaging tests
# Create a fixture for creating a local install of tolc to test
set(test_package ${CMAKE_CURRENT_BINARY_DIR}/test_package/tolc)
add_test(
  NAME InstallPackage
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --install ${CMAKE_BINARY_DIR} --prefix
          ${test_package})
set_tests_properties(InstallPackage PROPERTIES FIXTURES_SETUP
                                               FixtureInstallPackage)
message(
  STATUS "Using the local installation of tolc for tests: ${test_package}")

include(${PROJECT_SOURCE_DIR}/cmake/GetEmscripten.cmake)

# Will be used to compile wasm tests
if(NOT emsdk_SOURCE_DIR)
  get_emscripten(VERSION 3.1.3)
endif()
# Export the toolchain so it can be used in system tests
set(emscripten_toolchain
    ${emsdk_SOURCE_DIR}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
)

message(
  STATUS "Using the Emscripten toolchain for tests: ${emscripten_toolchain}")

add_subdirectory(systemTests)
add_subdirectory(moduleTests)
