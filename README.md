![Ubuntu](https://github.com/Tolc-Software/tolc/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Tolc-Software/tolc/workflows/MacOS/badge.svg) ![Windows](https://github.com/Tolc-Software/tolc/workflows/Windows/badge.svg)

# `Tolc` #

`Tolc` is a tool for automatically creating bindings from an existing `C++` library to another language. `Tolc` does not require you to change any of your existing `C++` interface. It should feel natural to use it from other languages. `Tolc` works with any compiler, and on Linux, MacOS, and Windows.

This repository only holds the command line interface for `tolc`, see [the Parser](https://github.com/Tolc-Software/Parser) for how the `C++` to be translated is parsed, or [one of the language modules](https://github.com/Tolc-Software/frontend.py) to see how the output is written. You can find the official documentation for tolc over at [docs.tolc.io](https://docs.tolc.io).

## Usage ##

`Tolc` provides convenient `CMake` helpers and is usually downloaded via the [`CMake` bootstrapper](https://github.com/Tolc-Software/bootstrap-tolc-cmake). It is as easy as putting the following in your `CMakeLists.txt`:

```cmake
# Download bootstrapper
include(FetchContent)
FetchContent_Declare(
  tolc_bootstrap
  GIT_REPOSITORY https://github.com/Tolc-Software/bootstrap-tolc-cmake
  GIT_TAG        main
)
FetchContent_MakeAvailable(tolc_bootstrap)

# Downloads and uses locates tolc
get_tolc()

# From the tolc package
# Creates the target MyLib_python for the CPython library.
tolc_create_translation(
  # Target to translate from
  TARGET MyLib
  # Language to target
  LANGUAGE python
  # Where to put the bindings
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python-bindings
)
```

`tolc_create_translation` wraps the `tolc` executable and makes sure that the `CPython` library that `python` understands gets built correctly.

With a simple enough interface, you can call the executable from this repository directly;

```shell
$ ./tolc python --input myLib.hpp --module-name MyLib --output python-bindings -I include
```

This will create a bindings file in the directory `python-bindings` for the header `myLib` with the include directory `include`. Your `C++` functions and classes can then be used from python as `import MyLib`.

See the [system tests](./tests/packaging/systemTests/) for more examples.

## Building ##

`Tolc` can be built on Linux, MacOS, and Windows. See the [CI pipelines for more platform specific information.](./.github/workflows/).

Here is a general overview:

Requirements:

* CMake
* python3
* conan
* clang

Configure the project:

```shell
$ cmake -S. -Bbuild -G Ninja -DCMAKE_CXX_COMPILER="clang++" -DCMAKE_C_COMPILER="clang" -DCMAKE_BUILD_TYPE="Debug" -DENABLE_TESTING=ON -DENABLE_PACKAGING=ON
```

Build `tolc`:

```shell
cmake --build build
```

Install with `CPack`:

```shell
$ cpack -G TGZ --config build/CPackConfig.cmake
```

