# Objective-C with Tolc #

In order for `C++` to be called from `Objective-C` there has to be an interface level. `Tolc` generates this level from your already written `C++` interface.
This is then generated to the appropriate targets by the `CMake` plugin.

## Requirements ##

* MacOS (uses `Foundation` headers)

## Using a `C++` library from `Objective-C` ##

This is a quick guide to using a `C++` library (here called `MyLib`) from `Objective-C`. We will:

1. Download `Tolc`
2. Use `Tolc` to generate bindings to an existing library
3. Use the `C++` library from both `Objective-C`

The following works on all supported platforms. On all platforms you need `git` available in your `path`. Commands that should be run from a terminal starts with `$ `, while comments starts with `# `.

### Downloading and using `Tolc` ###

Just add the following in a `CMakeLists.txt` below where the library you intend to use is defined:

```cmake
# Download Tolc
# Can be [latest, v0.7.0, ...]
set(tolc_version latest)
include(FetchContent)
FetchContent_Declare(
  tolc_entry
  URL https://github.com/Tolc-Software/tolc/releases/download/${tolc_version}/tolc-${CMAKE_HOST_SYSTEM_NAME}.tar.xz
)
FetchContent_Populate(tolc_entry)

set(tolc_DIR ${tolc_entry_SOURCE_DIR}/lib/cmake/tolc)
find_package(
  tolc
  CONFIG
  REQUIRED
)

tolc_create_bindings(
  TARGET MyLib
  LANGUAGE objc
  OUTPUT objc-bindings
)
```

Assuming your library is called `MyLib`, and the bindings should be generated to the directory `objc-bindings`.
This will also create a build target as `MyLib_objc` that you can treat as any other target.

Now you can configure your project as normal (in Visual Studio, this is typically done automatically on save):

```shell
$ cmake -S. -Bbuild
```

And finally build it:

```shell
$ cmake --build build
```

This will produce an `Objective-C` library under `build/tolc` (with `MSVC` it will be under for example `build/tolc/Debug`).

In order to use it we create an `Objective-C` application and link it to our libraries:

```cmake
add_executable(Objecty objc/main.m)
target_link_libraries(Objecty PRIVATE MyLib_objc)
```

The `Tolc` `CMake` plugin has created targets that take care of the rest.

From `Objective-C`:

```objc
#import <Mylib.h>

int main() {
  MyLibMyCppClass* myCppClass = [[MyLibMyCppClass alloc] init];
  [myCppClass someMethod];
}
```

If you want to see what more is supported you can take a look at [the Examples section](./examples.md).

