# `Tolc` `CMake` installation package #

This directory contains the `CMake`- and other scripts that will be shipped together with the installation package. The main entry point is the `tolc/tolcConfig.cmake`. This is what will get run whenever the user successfully runs `find_package(tolc CONFIG)`.

## Coding standards ##

### Naming ###

The naming convention follows the [`tolc-meta`](https://github.com/srydell/tolc-meta/blob/master/docs/cmake_coding_conventions.md take on `CMake` coding standards.) However, since these will be exposed within the global namespace of a customer project, some additional restrictions apply:

1. Exposed functions should be prefixed with `tolc_`, e.g. `tolc_translate_file`
2. Internal functions should be prefixed with `_tolc_`, e.g. `_tolc_fetch_internals`

### Generated files ###

Generated files that the user has not specified a location for must be placed under `${CMAKE_CURRENT_BINARY_DIR}/tolc`. This is to avoid name collision within the users build directory.
