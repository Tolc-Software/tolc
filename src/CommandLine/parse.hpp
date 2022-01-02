#pragma once

#include <optional>
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

	// The language chosen by the user to translate to
	std::string language;

	// The name that will be used when importing the module from some other language
	std::string moduleName;

	// Include directories for the input file
	std::vector<std::string> includes;

	// Was the help flag set? Then the parsing is successful,
	// but the objects are default constructed
	bool isHelp = false;

	// Do not send any analytics
	bool noAnalytics = false;
};

/**
* Parses the input and tries to create a CLIResult
* Returns std::nullopt on any non-compliant CLI options.
* To see what is compliant, run the executable with the --help flag
*/
[[nodiscard]] std::optional<CommandLine::CLIResult> parse(int argc,
                                                          const char** argv);
}
