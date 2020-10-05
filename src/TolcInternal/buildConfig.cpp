#include "TolcInternal/buildConfig.hpp"
#include "TolcInternal/getSystemIncludes.hpp"
#include <filesystem>
#include <optional>

namespace TolcInternal {

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

}    // namespace TolcInternal
