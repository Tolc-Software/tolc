# Using an existing C++ library from python #

In this tutorial we are going to use `tolc` to create `python` bindings to the `C++` library [tolc-demo](https://github.com/Tolc-Software/tolc-demo). Start by cloning the library

```shell
git clone https://github.com/Tolc-Software/tolc-demo.git
cd tolc-demo
```

In the [`include/Demo/demo.hpp`](https://github.com/Tolc-Software/tolc-demo/blob/main/include/Demo/demo.hpp) we can find the API that we want to use from `python`

```cpp
// include/Demo/demo.hpp
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

namespace Demo {
int add(int a, int b);
int sum(std::vector<int> const &numbers);
std::map<std::string, int> merge(std::map<std::string, int> const &m1,
                                 std::map<std::string, int> const &m2);
std::set<std::string> difference(std::set<std::string> const &s1,
                                 std::set<std::string> const &s2);
}
```

We can see that the library is built using `CMake` by the presence of the `CMakeLists.txt` file in the project root. Within this file, the library `Math` is defined

```cmake
# CMakeLists.txt
add_library(Math src/Demo/demo.cpp)
target_include_directories(Math PUBLIC include)
```

Below this code we download and install `tolc` locally into the project using [the bootstrapper](https://github.com/Tolc-Software/bootstrap-tolc-cmake)

```cmake
# CMakeLists.txt
include(FetchContent)
FetchContent_Declare(
  tolc_bootstrap
  GIT_REPOSITORY https://github.com/Tolc-Software/bootstrap-tolc-cmake
  GIT_TAG        main
)

FetchContent_MakeAvailable(tolc_bootstrap)
get_tolc()
```

After the call to `get_tolc()` we are free to use the `CMake` functions available in the `tolc` installation. To create bindings for `Math` we have to call the `tolc_create_translation` function

```cmake
# CMakeLists.txt
tolc_create_translation(
  TARGET Math
  LANGUAGE python
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/python-bindings
)
```

This will read the public API of `Math`, create bindings to `python` and put them in `${CMAKE_CURRENT_BINARY_DIR}/python-bindings` (typically `build/python-bindings`). It also created the target `Math_python` (`<TARGET>_<LANGUAGE>`) for the bindings, and is just a normal build target. Configuring the project

```shell
cmake -S. -Bbuild -DPYTHON_EXECUTABLE=$(which python3)
```

Note that the `-DPYTHON_EXECUTABLE=$(which python3)` is only strictly necessary on MacOS since it might otherwise use the system install of `python`. We can build the project

```shell
cmake --build build
```

Now `tolc` has read the public API of `Math`, created a set of `python` bindings, and then we used our normal compiler to build a `CPython` extension. We can now find and use the extension from `build/tolc`

```shell
cd build/tolc
python3
>>> import Math
>>> Math.Demo.merge({"tolc": 0}, {"demo": 1})
{'demo': 1, 'tolc': 0}
```

We see that our `C++` namespaces translates to `python` submodules, and our `python` dictionaries  cleanly translates into `C++`'s `std::map`.

We can now call all the public `C++` functions of `Math` from `python`.