#include "CommandLine/parse.hpp"
#include <CLI/CLI.hpp>
#include <optional>
#include <string>
#include <vector>

namespace CommandLine {

namespace {
/**
* This is to deal with CLI11.
* It assumes that a failed parse -> return non zero values.
* This wraps this functionality and just returns the exit status from this function
*/
int parseInternal(CLI::App& app,
                  int argc,
                  const char** argv) {
	// Will return if something goes wrong
	// Therefore it has to be at this level
	CLI11_PARSE(app, argc, argv);
	return 0;
}
}    // namespace

void addCommonCommands(CLI::App& languageApp, CommandLine::CLIResult& result) {
	// Adding directly to languageApp since these options
	// should come after the language subcommand in the CLI
	languageApp
	    .add_option("-i,--input",
	                result.inputFile,
	                "The interface file to be translated.")
	    ->required();

	languageApp
	    .add_option("-m,--module-name",
	                result.moduleName,
	                "The name of the exported library.")
	    ->required();

	languageApp
	    .add_option("-o,--output",
	                result.outputDirectory,
	                "The output directory where the bindings will be stored.")
	    ->required();

	languageApp.add_option(
	    "-I",
	    result.includes,
	    "Path to search for when resolving #include statements.");

	languageApp.add_flag(
	    "--no-analytics", result.noAnalytics, "Don't gather analytics.");
}

/**
* Adds the python subcommand with hooks to the rootApp
*/
[[nodiscard]] CLI::App* addPythonCommands(CLI::App& rootApp,
                                          CommandLine::CLIResult& result) {
	auto* python = rootApp.add_subcommand(
	    "python", "Create bindings to use C++ from python via CPython");

	addCommonCommands(*python, result);

	python->callback([&result]() { result.language = "python"; });

	return python;
}

/**
* Adds the wasm subcommand with hooks to the rootApp
*/
[[nodiscard]] CLI::App* addWasmCommands(CLI::App& rootApp,
                                        CommandLine::CLIResult& result) {
	auto* wasm = rootApp.add_subcommand(
	    "wasm", "Create bindings to use C++ from javascript via WebAssembly");

	addCommonCommands(*wasm, result);

	wasm->callback([&result]() { result.language = "wasm"; });

	return wasm;
}

/**
* Add options according to CLI11, and add hooks so that the values will be stored in result
*/
[[nodiscard]] std::vector<CLI::App*>
addSubcommandsAndOptions(CLI::App& app, CommandLine::CLIResult& result) {
	// One language must be chosen
	std::vector<CLI::App*> apps = {&app};

	apps.push_back(addPythonCommands(app, result));
	apps.push_back(addWasmCommands(app, result));

	// The number of subcommands added (as of writing only python => 1)
	app.require_subcommand(1);

	return apps;
}

[[nodiscard]] std::optional<CommandLine::CLIResult> parse(int argc,
                                                          const char** argv) {
	CLI::App app {
	    "Tolc is a bindings compiler between C++ and other languages"};

	CommandLine::CLIResult result;
	auto apps = CommandLine::addSubcommandsAndOptions(app, result);

	if (auto exitStatus = parseInternal(app, argc, argv); exitStatus == 0) {
		// Final check if CLI11 exited early due to a --help flag
		for (auto const& a : apps) {
			result.isHelp = result.isHelp || !a->get_help_ptr()->empty();
		}
		return result;
	}
	return std::nullopt;
}

}    // namespace CommandLine
