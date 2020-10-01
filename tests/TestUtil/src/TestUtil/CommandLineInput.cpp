#include "TestUtil/CommandLineInput.hpp"
#include <sstream>
#include <string>
#include <vector>

namespace TestUtil {

std::vector<std::string> split(std::string const& input, char delimiter = ' ') {
	std::istringstream iss(input);
	std::string token;
	std::vector<std::string> out;
	while (std::getline(iss, token, delimiter)) {
		out.push_back(token);
	}

	return out;
}

CommandLineInput::CommandLineInput(std::string const& input) {
	argc = 0;

	auto splitInput = split(input);
	argv = new const char*[splitInput.size()];
	for (auto const& str : splitInput) {
		char* next = new char[str.size() + 1];
		std::copy(str.begin(), str.end(), next);
		next[str.size()] = '\0';

		argv[argc] = next;
		++argc;
	}
}

CommandLineInput::~CommandLineInput() {
	for (int i = 0; i < argc; ++i) {
		delete argv[i];
	}
	delete argv;
}
}    // namespace TestUtil
