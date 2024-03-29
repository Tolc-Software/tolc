# CMake Reference #

# `tolc_create_bindings` #

Creates a target that is the library that can be consumed by some other language. The resulting target name is `${TARGET}_${LANGUAGE}` (e.g. `MyCppLib_python`).

## Parameters ##

* `TARGET`

Specifies the target to be translated. By default it will search the `PUBLIC`/`INTERFACE` directories for headers to translate. Will link the created target with this input.


* `LANGUAGE`

The language to translate to. Will determine the output format of `tolc`. Will also determine any dependencies to be downloaded (in the case of `python`, `pybind11` will be downloaded locally to the build directory).


* `OUTPUT`

Determines the output directory for the bindings.

* `HEADERS`

Takes a list of headers relative the current file. Adds headers to be translated. Useful for adding forward declarated templates or together with `DO_NOT_SEARCH_TARGET_INCLUDES` to declare which part of the public interface to translate.

* `DO_NOT_SEARCH_TARGET_INCLUDES`

Disable searching the public interface of `TARGET` for header files. This can be used in tandem with `HEADERS` to only translate a specific part of the public interface.

* `NO_ANALYTICS`

Disable analytics.

## Example ##

```cmake
tolc_create_bindings(
  TARGET MyLib
  LANGUAGE python
  HEADERS
    include/MyLib.hpp
    include/myForwardDeclarations.hpp
  DO_NOT_SEARCH_TARGET_INCLUDES
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python-bindings
)
```

