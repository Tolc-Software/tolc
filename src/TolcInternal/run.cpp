#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <Frontend/Python/frontend.hpp>
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fstream>

namespace TolcInternal {

/**
  * Check the language passed in an call the appropriate frontend on the input namespace
  */
std::pair<std::filesystem::path, std::string>
callFrontend(TolcInternal::Config::Language language,
             IR::Namespace const& globalNamespace) {
	std::filesystem::path file;
	std::string content;
	switch (language) {
		case TolcInternal::Config::Language::Python:
			std::tie(file, content) =
			    Frontend::Python::createModule(globalNamespace);
			break;
	}
	return {file, content};
}

int run(int argc, const char** argv) {
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
				auto globalNamespace = maybeGlobalNamespace.value();
				globalNamespace.m_name = config.moduleName;

				auto [file, content] =
				    callFrontend(config.language, globalNamespace);

				std::filesystem::create_directories(config.outputDirectory);
				std::ofstream outFile;
				outFile.open(config.outputDirectory / file);
				if (outFile.is_open()) {
					outFile << content;
				}

				return 0;
			}
		}
	}

	return 1;
}

}    // namespace TolcInternal
