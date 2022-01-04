# Testing packages #

This part of the test suite is meant to test each part of the installation sent to a user. It is split into two part, `systemTests` and `moduleTests`.

**NOTE:** The `CMakeLists.txt` at this level also provides access to an install of the `tolc` version in this repo automatically.

**NOTE**: In order for this to work, packaging obviously needs to be turned on (should be handled on a parent level).

## System tests ##

These simulate a full project that will be using a tolc installation. It should be as close as possible to a user project.

## Module tests ##

An installation of `tolc` comes with a set of `CMake` help modules that are used to take information from the build system and give it to `tolc`. These tests are meant as unit tests for these modules.
