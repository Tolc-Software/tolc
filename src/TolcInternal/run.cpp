#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "Log/log.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <Frontend/Python/frontend.hpp>
#include <Frontend/Wasm/frontend.hpp>
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
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
		case TolcInternal::Config::Language::Wasm:
			return Frontend::Wasm::createModule(globalNamespace, moduleName);
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
		outFile << fmt::format(
		    "#include <{}>\n{}", config.inputFile.string(), content);
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
			std::cout << "Managed to build config" << '\n';
			auto config = maybeConfig.value();

			// Try to parse it
			if (auto maybeGlobalNamespace =
			        Parser::parseFile(config.inputFile, config.parserConfig)) {
				std::cout << "Managed to parse code" << '\n';
				if (auto output = callFrontend(config.language,
				                               maybeGlobalNamespace.value(),
				                               config.moduleName)) {
					std::cout << "Managed to call frontend" << '\n';
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
