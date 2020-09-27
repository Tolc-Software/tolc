#include "CommandLine/addOptions.hpp"
#include <CLI/CLI.hpp>
#include <string>
#include <vector>

namespace CommandLine {

void addCommandLineOptions(CLI::App& app, CommandLine::CLIResult& result) {
	// One language must be chosen
	app.require_subcommand(1);

	auto* python =
	    app.add_subcommand("python", "Translate input for use from python");

	// Adding directly to python since these options
	// should come after the language subcommand in the CLI
	python
	    ->add_option("-i,--input",
	                 result.inputFile,
	                 "The interface file to be translated.")
	    ->required();
	std::string outputDirectoryOption;
	python
	    ->add_option("-o,--output",
	                 result.outputDirectory,
	                 "The output directory where the bindings will be stored.")
	    ->required();

	python->callback([&result]() {
		result.language = "python";

		std::cout << "Got the python command" << '\n';
		std::cout << "Got input: " << result.inputFile << '\n';
		std::cout << "Got output: " << result.outputDirectory << '\n';
	});
}

}    // namespace CommandLine
