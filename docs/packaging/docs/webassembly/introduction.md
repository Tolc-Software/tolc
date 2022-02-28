# WebAssembly with Tolc #

In order for `C++` to be called from `javascript` there has to be an interface level. `tolc` generates this level from your already written `C++` interface.
To be as close to what an engineer would have written, `tolc` generates human readable [`embind11`](https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#embind).
This is then compiled to a `.wasm` and a `.js` file that `javascript` can import.

## Using a `C++` library from `javascript` ##

This is a quick guide to using a `C++` library (here called `MyLib`) from `javascript`. We will:

1. Download and use `Tolc`
2. Download and set up `Emscripten`
3. Use the resulting `WebAssembly` from `javascript`

The following works on all supported platforms. On all platforms you need `git` available in your `path`. Commands that should be run from a terminal starts with `$ `, while comments starts with `# `.

### Downloading `Tolc` ###

Just add the following in a `CMakeLists.txt` below where the library you intend to use from `javascript` is defined:

```cmake
# Download Tolc
# Can be ["latest", "v0.2.0", ...]
set(tolc_version latest)
include(FetchContent)
FetchContent_Declare(
  tolc_entry
  URL https://github.com/Tolc-Software/tolc/releases/download/${tolc_version}/tolc-${CMAKE_HOST_SYSTEM_NAME}.tar.xz
)
FetchContent_Populate(tolc_entry)

find_package(
  tolc
  CONFIG
  PATHS
  ${tolc_entry_SOURCE_DIR}
  REQUIRED)

tolc_create_bindings(
  TARGET MyLib
  LANGUAGE wasm
  OUTPUT wasm-bindings)
```

Assuming your library is called `MyLib`, and the bindings should be generated to the directory `wasm-bindings`.

### Downloading `Emscripten` ###

In order to compile your library to `WebAssembly`, you need to download the [`Emscripten compiler`](https://emscripten.org/). This is typically done via the `Emscripten SDK`. Navigate to the directory where you want to install and run the following commands:

```shell
# Download SDK
$ git clone https://github.com/emscripten-core/emsdk.git
$ cd emsdk
```

Now follow the specifig commands for your platform.

#### Linux/MacOS ####

From within the `emsdk` directory:

```shell
# Download and install locally
$ ./emsdk install 3.1.3
# Writes configuration file .emscripten
$ ./emsdk activate 3.1.3
```

#### Windows ####

From within the `emsdk` directory:

```shell
# Download and install locally
$ emsdk.bat install 3.1.3
# Writes configuration file .emscripten
$ emsdk.bat activate 3.1.3
```

### Configuring Your Project ###

Now when configuring your `CMake` project, pass the toolchain flag `-DCMAKE_TOOLCHAIN_FILE=${EMSDK_DIRECTORY}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake`. Where you need to replace `${EMSDK_DIRECTORY}` with the directory of the previously downloaded `Emscripten SDK`. Note that the directory separator used by `CMake` is always forward slash (`/`), even on Windows.

Example:

```shell
# Configures project to build using Emscripten
$ cmake -S. -Bbuild -DCMAKE_TOOLCHAIN_FILE=${EMSDK_DIRECTORY}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake
```

### Using From `javascript` ###

Looking into `build/tolc` you should see `MyLib.js` aswell as `MyLib.wasm`. `MyLib.js` exports a `Promise` that loads the built `WebAssembly`. Here is an example usage:

```javascript
// run.js
const loadMyLib = require('./build/MyLib');

loadMyLib().then(MyLib => {
  // From here you can use the C++ functions of your library as usual
  MyLib.myCppFunction();
});
```

If you want to see what more is supported you can take a look at [the Examples section](./examples.md).

