# Installing

The recommended and easiest way is to use the prebuilt binaries under [the release page](https://github.com/Tolc-Software/tolc/releases). To download the appropriate one for your platform you can just drop the following lines in your `CMake` project.

```CMake
# Can be ["latest", "v0.2.0", ...]
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
```

When reconfiguring your project, the `Tolc` `CMake` interface should be available automatically. Note that this requires `CMake 3.15` or later.
