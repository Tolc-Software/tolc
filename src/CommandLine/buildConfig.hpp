#pragma once

#include "CommandLine/Config.hpp"
#include <optional>

namespace CommandLine {
/**
* Reads the argc, argv and provides a valid config
*/
std::optional<CommandLine::Config> buildConfig(int argc, const char** argv);
}    // namespace CommandLine
