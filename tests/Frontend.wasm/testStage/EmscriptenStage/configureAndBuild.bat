@echo off
rem   ^-Do not echo out every command

rem One or more of these will not work, they are in reverse priority order
rem It is too difficult writing bat files.
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars64.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvars64.bat"

cmake -S. -Bbuild -G Ninja -DCMAKE_C_COMPILER=clang-cl -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_BUILD_TYPE=Release

cmake --build build -j 1
