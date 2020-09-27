#pragma once

#include <CLI/CLI.hpp>
#include <string>
#include <vector>

namespace CommandLine {

/**
* Will store the result after parsing the command line options given to tolc
*/
struct CLIResult {
	// What file should be translated
	std::string inputFile;

	// Where tolc is allowed to put output
	std::string outputDirectory;

	// Include directories for the input file
	std::vector<std::string> includes;
};

/**
* Set what is valid input to the tolc executable
*/
void addCommandLineOptions(CLI::App& app, CommandLine::CLIResult& result);
}
