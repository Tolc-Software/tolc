## Good to know ##

When building with `pybind11` at the moment of writing, it can sometimes find the wrong `python` version. This is because it is using an old findPython `CMake` module internally. [As can be seen in this open issue on Github](https://github.com/pybind/pybind11/issues/2154). **THE VERSION OF PYBIND11 THAT IS USED INTERNALLY SHOULD BE UPDATED AS SOON AS THIS ISSUE IS CLOSED.** Meanwhile, the workaround is to explicitly pass in `-DPYTHON_EXECUTABLE` to the configure stage of that project. This is currently done when doing system tests as:


```cmake
find_package(Python3 REQUIRED)

...

  add_test(
    NAME Generate${systemTestName}
    WORKING_DIRECTORY ${consumer}
    COMMAND
      ${CMAKE_COMMAND} -S. -Bbuild -GNinja
      -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}
      -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_BUILD_TYPE=Debug -Dtolc_ROOT=${test_package} -DPYTHON_EXECUTABLE=${Python3_EXECUTABLE})

...
```


