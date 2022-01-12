# Installing

The recommended and easiest way is to use the prebuilt binaries under [the release page](https://github.com/Tolc-Software/tolc/releases/tag/main-release). To download the appropriate one for your platform you can just drop the following lines in your `CMake` project.

```CMake
include(FetchContent)
FetchContent_Declare(
  tolc_entry
  URL https://github.com/Tolc-Software/tolc/releases/download/main-release/tolc-${CMAKE_HOST_SYSTEM_NAME}-main.tar.gz
)
FetchContent_Populate(tolc_entry)

find_package(
  tolc
  CONFIG
  PATHS
  ${tolc_entry_SOURCE_DIR}
  REQUIRED
  NO_DEFAULT_PATH)
```

When reconfiguring your project, the `Tolc` `CMake` interface should be available automatically. Note that this requires `CMake 3.15` or later.