#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <Frontend/Python/frontend.hpp>
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fstream>

#include <chrono>
#include <iostream>

namespace TolcInternal {

/**
  * Check the language passed in an call the appropriate frontend on the input namespace
  */
std::pair<std::filesystem::path, std::string>
callFrontend(TolcInternal::Config::Language language,
             IR::Namespace const& globalNamespace,
             std::string const& moduleName) {
	std::filesystem::path file;
	std::string content;
	switch (language) {
		case TolcInternal::Config::Language::Python:
			std::tie(file, content) =
			    Frontend::Python::createModule(globalNamespace, moduleName);
			break;
	}
	return {file, content};
}

/**
* Logs the time taken from start
*/
void logTimeTaken(decltype(std::chrono::high_resolution_clock::now()) start) {
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration =
	    std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
	std::cout << duration.count() << '\n';
}

int run(int argc, const char** argv) {
	// Used to time execution
	auto start = std::chrono::high_resolution_clock::now();

	if (auto maybeResult = CommandLine::parse(argc, argv)) {
		auto cliResult = maybeResult.value();
		// Check if user asked for --help
		if (cliResult.isHelp) {
			return 0;
		}

		// Validate user input
		if (auto maybeConfig = buildConfig(cliResult)) {
			auto config = maybeConfig.value();

			// Try to parse it
			if (auto maybeGlobalNamespace =
			        Parser::parseFile(config.inputFile, config.parserConfig)) {
				auto [file, content] =
				    callFrontend(config.language,
				                 maybeGlobalNamespace.value(),
				                 config.moduleName);

				std::filesystem::create_directories(config.outputDirectory);
				std::ofstream outFile;
				outFile.open(config.outputDirectory / file);
				if (outFile.is_open()) {
					// Inject the input file aswell
					outFile << "#include <" << config.inputFile.string() << ">\n"
					        << content;
				}

				logTimeTaken(start);
				return 0;
			}
		}
	}

	logTimeTaken(start);
	return 1;
}

}    // namespace TolcInternal
