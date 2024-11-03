#pragma once

#include <filesystem>

namespace TestStage {

/**
* Get the path to the root of test stage
* NOTE: This is populated at configure time by cmake
*/
std::filesystem::path getRootStagePath();

/**
* Get the path to the examples directory
* NOTE: This is populated at configure time by cmake
*/
std::filesystem::path getExamplesPath();
}
