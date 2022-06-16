#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "Log/log.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <Frontend/Objc/frontend.hpp>
#include <Frontend/Python/frontend.hpp>
#include <Frontend/Wasm/frontend.hpp>
#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <spdlog/spdlog.h>

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
		case TolcInternal::Config::Language::Wasm:
			return Frontend::Wasm::createModule(globalNamespace, moduleName);
		case TolcInternal::Config::Language::Objc:
			return Frontend::Objc::createModule(globalNamespace, moduleName);
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
	auto outPath = config.outputDirectory / file;
	outFile.open(outPath);
	if (outFile.is_open()) {
		if (outPath.extension().string() == ".cpp" ||
		    outPath.extension().string() == ".mm") {
			// Inject the input file aswell
			outFile << fmt::format("#include <{}>\n",
			                       config.inputFile.string());
		}
		outFile << content;
	}
}

enum class ErrorOrigin {
	Parser,
	FrontendPy,
	FrontendWasm,
	FrontendObjc,
};

ErrorOrigin getOrigin(Config::Language language) {
	switch (language) {
		case Config::Language::Python: return ErrorOrigin::FrontendPy;
		case Config::Language::Wasm: return ErrorOrigin::FrontendWasm;
		case Config::Language::Objc: return ErrorOrigin::FrontendObjc;
	}
	// Should never happen :)
	return ErrorOrigin::Parser;
}

void logError(ErrorOrigin origin) {
	std::string repository;
	switch (origin) {
		case ErrorOrigin::Parser: repository = "Parser"; break;
		case ErrorOrigin::FrontendPy: repository = "frontend.py"; break;
		case ErrorOrigin::FrontendWasm: repository = "frontend.wasm"; break;
		case ErrorOrigin::FrontendObjc: repository = "frontend.swift"; break;
	}
	spdlog::error(
	    "If this error is something Tolc can solve, please open a feature request or a bug report here: https://github.com/Tolc-Software/{}/issues/new",
	    repository);
}

int run(int argc, const char** argv) {
	Log::Data logData {};
	spdlog::set_pattern("[Tolc][%l]: %v");

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
				} else {
					logError(getOrigin(config.language));
				}
			} else {
				logError(ErrorOrigin::Parser);
			}
		}
	}

	Log::logTimeTaken(logData);
	return 1;
}

}    // namespace TolcInternal
