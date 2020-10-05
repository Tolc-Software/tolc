#pragma once
#include <filesystem>

namespace TolcInternal {
/*
* Get the path to the currently running executable
**/
std::filesystem::path getExecutablePath();
}
