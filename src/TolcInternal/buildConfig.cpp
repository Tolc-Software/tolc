#include "TolcInternal/buildConfig.hpp"
#include "TolcInternal/getExecutableDirectoryPath.hpp"
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

	config.moduleName = cli.moduleName;

	auto& parserConfig = config.parserConfig;
	// Add the includes from libcpp
	parserConfig.m_systemIncludes = TolcInternal::getSystemIncludes(
	    TolcInternal::getExecutableDirectoryPath());

	// Add the user includes
	for (auto const& include : cli.includes) {
		parserConfig.m_systemIncludes.emplace_back("-I" + include);
	}

	if (cli.language == "python") {
		config.language = Config::Language::Python;
	} else {
		return {};
	}

	return config;
}

}    // namespace TolcInternal
