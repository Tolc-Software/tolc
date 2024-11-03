# Test Stage #

## Overview ##

This directory does the following:

1. Download and stores for the `EmscriptenStage`:
  * Emscripten
  * jest
2. Acts as a simple testing ground for new features from `embind`.

It is therefore required as a test fixture to configure this project before creating a new test stage.

## In Depth ##

Test Stage is a basis for all the generated "projects" that will be used to test `frontend.wasm`. The files in here can be copied to a new directory and built together with the test source code. It is up to the test which files are copied. Therefore it is also used as a test bed for new features in `embind`.

Configuring this directory downloads and configures a known version of Emscripten. Then the files from ./EmscriptenStage can be used together with the downloaded toolchain;

```shell
# From a newly copied stage of EmscriptenStage
$ cmake -S. -Bbuild -GNinja -DCMAKE_TOOLCHAIN_FILE=/path/to/testStage/build/_deps/emsdk_entry-src/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
$ cmake --build build
```
