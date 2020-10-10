#pragma once
#include <filesystem>

namespace TolcInternal {
/*
* Get the directory path of the currently running executable
**/
std::filesystem::path getExecutableDirectoryPath();
}
