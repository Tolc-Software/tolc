# Installing

## Via the `CMake` bootstrapper

The easiest and recommended way is to use [the bootstrapper](https://github.com/Tolc-Software/bootstrap-tolc-cmake). Just drop the following lines in your `CMake` project.

```CMake
include(FetchContent)
FetchContent_Declare(
  tolc_bootstrap
  GIT_REPOSITORY https://github.com/Tolc-Software/bootstrap-tolc-cmake
  GIT_TAG        main
)

FetchContent_MakeAvailable(tolc_bootstrap)
# Download and use find_package to locate tolc
get_tolc()
```

When reconfiguring your project, the `tolc` `CMake` interface should be available automatically. Note that this requires `CMake 3.15` or later.

## From the release page

The latest package is available on [the release page](https://github.com/Tolc-Software/tolc-beta/releases/tag/beta-release). From here you can use the included binary or `CMake` interface to use it in your project.
