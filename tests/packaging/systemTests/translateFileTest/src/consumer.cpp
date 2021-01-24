#include "consumer.hpp"
#include "check_variables.hpp"
#include <filesystem>
#include <iostream>

int main() {
	if (!std::filesystem::exists(getExpectedOutFile())) {
		std::cerr << "Output file does not exist: " << getExpectedOutFile()
		          << '\n';
		return 1;
	}
	return 0;
}
