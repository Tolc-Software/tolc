# Module tests #

The module tests are split into two two categories; the ones which should fail, and the ones which should succeed. Failing or succeeding is always based on exit code, so running a `CMake` module containing a failing function results in a non-zero exit code.

## Help module ##

There is a generated help module that contains the path to a `tolc` installation. It can be included into a test to make all the installed modules available.

**NOTE:** The actual installation package that gets tested is generated at a parent level since it is reused over several tests.
