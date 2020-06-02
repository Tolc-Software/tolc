#include "CommandLine/buildConfig.hpp"
#include "CommandLine/Config.hpp"
#include "CommandLine/LanguageConfig/python.hpp"
#include <Frontend/Python/frontend.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <iostream>
#include <lyra/lyra.hpp>
#include <optional>

namespace CommandLine {

std::optional<CommandLine::Config> buildConfig(int argc, const char** argv) {
	auto cli = lyra::cli_parser();
	std::string language;
	bool show_help = false;
	cli.add_argument(lyra::help(show_help));
	cli.add_argument(lyra::arg(language, "language")
	                     .choices(LanguageConfig::Python::name())
	                     .required()
	                     .help("language to perform."));
	auto result = cli.parse({argc, argv});
	if (show_help) {
		std::cout << cli;
		return {};
	}

	if (language == LanguageConfig::Python::name()) {
		LanguageConfig::Python p(cli);
		result = cli.parse({argc, argv});
		if (result) {
			CommandLine::Config c;
			std::cout << "Got input: " << p.inputFile << '\n';
			std::cout << "Got output: " << p.outputDirectory << '\n';
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
