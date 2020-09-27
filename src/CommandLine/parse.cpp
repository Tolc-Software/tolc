#include "CommandLine/parse.hpp"
#include <CLI/CLI.hpp>
#include <string>
#include <vector>

namespace CommandLine {

namespace {
/**
* This is to deal with CLI11.
* It assumes that a failed parse -> return non zero values.
* This wraps this functionality and just returns the exit status from this function
*/
int parseInternal(CLI::App& app, int argc, const char** argv) {
	// Will return if something goes wrong
	// Therefore it has to be at this level
	CLI11_PARSE(app, argc, argv);
	return 0;
}
}    // namespace

/**
* Add options according to CLI11, and add hooks so that the values will be stored in result
*/
void addOptions(CLI::App& app, CommandLine::CLIResult& result) {
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

	python
	    ->add_option("-o,--output",
	                 result.outputDirectory,
	                 "The output directory where the bindings will be stored.")
	    ->required();

	python->add_option(
	    "-I",
	    result.includes,
	    "Path to search for when resolving #include statements.");

	python->callback([&result]() {
		result.language = "python";
	});
}

std::optional<CommandLine::CLIResult> parse(int argc, const char** argv) {
	CLI::App app {"Tolc is an automatic translator for C++"};

	CommandLine::CLIResult result;
	CommandLine::addOptions(app, result);

	if (auto exitStatus = parseInternal(app, argc, argv); exitStatus == 0) {
		return result;
	}
	return std::nullopt;
}

}    // namespace CommandLine
