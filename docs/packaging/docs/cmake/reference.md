# `tolc_create_translation` #

Creates a target that is the library that can be consumed by some other language.

## Parameters ##

* `TARGET`

Specifies the target to be translated. By default it will search the `PUBLIC`/`INTERFACE` directories for headers to translate. Will link the created target with this input.


* `LANGUAGE`

The language to translate to. Will determine the output format of `tolc`. Will also determine any dependencies to be downloaded (in the case of `python`, `pybind11` will be downloaded locally to the build directory).


* `OUTPUT`

Determines the output directory for the bindings.

## Example ##

```cmake
tolc_create_translation(
  TARGET MyLib
  LANGUAGE python
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python-bindings
)
```

