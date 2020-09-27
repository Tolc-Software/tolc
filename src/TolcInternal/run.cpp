#include "TolcInternal/run.hpp"
#include "CommandLine/addOptions.hpp"
#include <CLI/CLI.hpp>
#include <filesystem>
#include <string>
#include <vector>

namespace TolcInternal {


int run(int argc, const char** argv) {
	CLI::App app {"Tolc is an automatic translator for C++"};

	CommandLine::CLIResult result;
	CommandLine::addCommandLineOptions(app, result);

	// Will return if something goes wrong
	// Therefore it has to be at this level
	CLI11_PARSE(app, argc, argv);

	return 0;
}

}    // namespace TolcInternal
