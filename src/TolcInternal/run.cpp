#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <Frontend/Python/frontend.hpp>
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fstream>
#include "Log/log.hpp"
#include <chrono>
#include <fmt/format.h>
#include <iostream>

namespace TolcInternal {

/**
  * Check the language passed in an call the appropriate frontend on the input namespace
  */
std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
callFrontend(TolcInternal::Config::Language language,
             IR::Namespace const& globalNamespace,
             std::string const& moduleName) {
	switch (language) {
		case TolcInternal::Config::Language::Python:
			return Frontend::Python::createModule(globalNamespace, moduleName);
			break;
	}
	return std::nullopt;
}

/**
* Write content to file
* Makes sure the directory the file is in exists
* Injects include to the input file
*/
void writeFile(TolcInternal::Config const& config,
               std::filesystem::path const& file,
               std::string const& content) {
	std::filesystem::create_directories(config.outputDirectory);
	std::ofstream outFile;
	outFile.open(config.outputDirectory / file);
	if (outFile.is_open()) {
		// Inject the input file aswell
		outFile << "#include <" << config.inputFile.string() << ">\n"
		        << content;
	}
}

int run(int argc, const char** argv) {
	Log::Data logData {};

	if (auto maybeResult = CommandLine::parse(argc, argv)) {
		auto cliResult = maybeResult.value();
		// Check if user asked for --help
		if (cliResult.isHelp) {
			return 0;
		}

		logData.noAnalytics = cliResult.noAnalytics;

		// Validate user input
		if (auto maybeConfig = buildConfig(cliResult)) {
			auto config = maybeConfig.value();

			// Try to parse it
			if (auto maybeGlobalNamespace =
			        Parser::parseFile(config.inputFile, config.parserConfig)) {
				if (auto output = callFrontend(config.language,
				                               maybeGlobalNamespace.value(),
				                               config.moduleName)) {
					for (auto const& [file, content] : output.value()) {
						writeFile(config, file, content);
					}
					logData.success = true;
					Log::logTimeTaken(logData);
					return 0;
				}
			}
		}
	}

	Log::logTimeTaken(logData);
	return 1;
}

}    // namespace TolcInternal
