#pragma once

#include <string>

namespace TestUtil {
struct CommandLineInput {
	int argc;
	const char** argv;

	CommandLineInput(std::string const& input);

	~CommandLineInput();
};
}    // namespace TestUtil
