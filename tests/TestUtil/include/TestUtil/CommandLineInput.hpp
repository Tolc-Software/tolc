#pragma once

#include <string>

namespace TestUtil {
/**
* Converts string input into argc and argv
*/
struct CommandLineInput {
	int argc;
	const char** argv;

	CommandLineInput(std::string const& input);

	~CommandLineInput();
};
}    // namespace TestUtil
