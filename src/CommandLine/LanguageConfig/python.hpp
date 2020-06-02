#pragma once

#include <filesystem>
#include <lyra/lyra.hpp>

namespace CommandLine::LanguageConfig {
struct Python {
	static char const* name() {
		return "python";
	}
	std::filesystem::path inputFile;
	std::filesystem::path outputDirectory;

	Python(lyra::cli_parser& cli) {
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
}    // namespace CommandLine::LanguageConfig
