# System tests #

This is not part of the core test suite for the project. It is meant for testing that a generated install of the project works for a consuming user. All tests here are automatic, but it is good to know the overall process that is being done:

1. Generate an installation package of tolc (done at the parent level)
2. Generate the consumer project (inject path to our installation)
3. Build the consumer project
4. Run the built executable (assumes the target name is `consumer`)

These need to be done in this order, and to do so [CMake test fixtures](https://cmake.org/cmake/help/latest/prop_test/FIXTURES_REQUIRED.html) is used.
