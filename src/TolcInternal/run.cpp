#include "TolcInternal/run.hpp"
#include "CommandLine/addOptions.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace TolcInternal {

/**
* Stores everyting needed to use the Parser and Frontends
*/
struct ConfigInternal {
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
std::optional<ConfigInternal> buildConfig(CommandLine::CLIResult const&) {
	std::optional<ConfigInternal> config = std::nullopt;

	// if (auto input = std::filesystem::path(cli.inputFile);
	//     std::filesystem::is_regular_file(input)) {
	// 	config.inputFile = input;
	// } else {
	// 	return {};
	// }

	// if (auto output = std::filesystem::path(cli.outputDirectory);
	//     std::filesystem::is_directory(output)) {
	// 	config.outputDirectory = output;
	// } else {
	// 	return {};
	// }

	// if (cli.language == "python") {
	// 	config.language = ConfigInternal::Language::Python;
	// } else {
	// 	return {};
	// }

	return config;
}

int run(int argc, const char** argv) {
	CLI::App app {"Tolc is an automatic translator for C++"};

	CommandLine::CLIResult result;
	CommandLine::addCommandLineOptions(app, result);

	if (auto maybeConfig = buildConfig(result)) {
		auto config = maybeConfig.value();
		std::cout << "Got config with input: " << config.inputFile << '\n';
	}

	// Will return if something goes wrong
	// Therefore it has to be at this level
	CLI11_PARSE(app, argc, argv);

	return 0;
}

}    // namespace TolcInternal
