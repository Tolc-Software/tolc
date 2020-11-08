# Testing packages #

This is not part of the core test suite for the project. It is meant for testing that a generated install of the project works for a consuming user. All tests here are automatic, but it is good to know the overall process that is being done:

1. Generate an installation package of tolc
2. Copy the generated package into the consumer project
3. Build the consumer project
4. Run the built executable

These need to be done in this order, and to do so [CMake test fixtures](https://cmake.org/cmake/help/latest/prop_test/FIXTURES_REQUIRED.html) is used.

**NOTE**: In order for this to work, packaging obviously needs to be turned on (should be handled on a parent level).
