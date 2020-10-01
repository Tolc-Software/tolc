#pragma once

#include <string>
#include <vector>

namespace TolcInternal {
/**
* Return a vector of platform specific include directories.
* The input is used as a prefix to the llvm includes
*/
std::vector<std::string> getSystemIncludes(std::string const& llvmRoot);
}
