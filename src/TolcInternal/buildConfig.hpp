#pragma once

#include "CommandLine/parse.hpp"
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace TolcInternal {
/**
* Stores everyting needed to use the Parser and Frontends
*/
struct Config {
	enum class Language { Python };

	// The language chosen to translate to
	Language language;

	// What file should be translated
	std::filesystem::path inputFile;

	// Where tolc is allowed to put output
	std::filesystem::path outputDirectory;

	// Include directories for the input file
	std::vector<std::filesystem::path> includes;
};

/**
* Tries to build the config from the cli options given by the user
*/
std::optional<Config> buildConfig(CommandLine::CLIResult const& cli);
}
