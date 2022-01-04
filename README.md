![Ubuntu](https://github.com/Tolc-Software/tolc/workflows/Ubuntu/badge.svg) ![MacOS](https://github.com/Tolc-Software/tolc/workflows/MacOS/badge.svg) ![Windows](https://github.com/Tolc-Software/tolc/workflows/Windows/badge.svg)

# `Tolc` #

`Tolc` is a tool for automatically creating bindings from an existing `C++` library to another language. `Tolc` does not require you to change any of your existing `C++` interface. It should feel natural to use it from other languages. `Tolc` works with any compiler, and on Linux, MacOS, and Windows.

This repository only holds the command line interface for `tolc`, see [the Parser](https://github.com/Tolc-Software/Parser) for how the `C++` to be translated is parsed, or [one of the language modules](https://github.com/Tolc-Software/frontend.py) to see how the output is written.

## Usage ##

`Tolc` provides convenient `CMake` helpers and is usually downloaded via the [`CMake` bootstrapper](https://github.com/Tolc-Software/bootstrap-tolc-cmake). That said, with a simple enough interface, you can just call the executable from this repository directly;

```shell
$ ./tolc python --input myLib.hpp --module-name MyLib --output python-bindings -I include
```

This will create a bindings file in the directory `python-bindings` for the header `myLib` with the include directory `include`. Your `C++` functions and classes can then be used from python as `import MyLib`.

See the [system tests](./tests/packaging/systemTests/) for more information.
