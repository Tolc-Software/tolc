#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>
#include <iostream>
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
std::optional<Config> buildConfig(CommandLine::CLIResult const& cli) {
	Config config;

	if (auto input = std::filesystem::path(cli.inputFile);
	    std::filesystem::is_regular_file(input)) {
		config.inputFile = input;
	} else {
		return {};
	}

	config.outputDirectory = std::filesystem::path(cli.outputDirectory);

	config.includes.reserve(cli.includes.size());
	for (auto const& include : cli.includes) {
		config.includes.emplace_back(include);
	}

	if (cli.language == "python") {
		config.language = Config::Language::Python;
	} else {
		return {};
	}

	return config;
}

int run(int argc, const char** argv) {
	if (auto maybeResult = CommandLine::parse(argc, argv)) {
		auto result = maybeResult.value();
		if (auto maybeConfig = buildConfig(result)) {
			auto config = maybeConfig.value();
			std::cout << "Got config with input: " << config.inputFile << '\n';
		}
	}

	return 1;
}

}    // namespace TolcInternal
