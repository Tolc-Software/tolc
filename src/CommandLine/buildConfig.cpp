#include "CommandLine/buildConfig.hpp"
#include "CommandLine/Config.hpp"
#include <Frontend/Python/frontend.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <lyra/lyra.hpp>
#include <optional>
#include <sstream>

namespace CommandLine {

namespace {
struct PythonConfig {
	static char const* name() {
		return "python";
	}
	std::filesystem::path inputFile;
	std::filesystem::path outputDirectory;

	PythonConfig(lyra::cli_parser& cli) {
		cli.add_argument(lyra::opt(inputFile, "input")
		                     .name("-i")
		                     .name("--input")
		                     .required()
		                     .help("The interface file to be parsed."));
		cli.add_argument(
		    lyra::opt(outputDirectory, "output")
		        .name("-o")
		        .name("--output")
		        .required()
		        .help(
		            "The output directory where the bindings will be stored."));
	}
};
}    // namespace

std::optional<CommandLine::Config> buildConfig(int argc, const char** argv) {
	auto cli = lyra::cli_parser();
	std::string language;
	bool show_help = false;
	cli.add_argument(lyra::help(show_help));
	cli.add_argument(lyra::arg(language, "language")
	                     .choices(PythonConfig::name())
	                     .required()
	                     .help("language to perform."));
	auto result = cli.parse({argc, argv});
	if (show_help) {
		std::cout << cli;
		return {};
	}

	if (language == PythonConfig::name()) {
		PythonConfig p(cli);
		result = cli.parse({argc, argv});
		if (result) {
			CommandLine::Config c;
			std::cout << "Got input: " << p.inputFile << '\n';
			std::cout << "Got output: " << p.outputDirectory << '\n';
			std::string fileBasename;
			std::stringstream ss(p.inputFile);
			std::getline(ss, fileBasename, '.');
			c.m_language = language;
			c.m_inputFile = p.inputFile;
			c.m_outputDirectory = p.outputDirectory;
			return c;
		}
	}
	if (!result) {
		std::cerr << result.errorMessage() << "\n";
	}
	return {};
}
}    // namespace CommandLine
