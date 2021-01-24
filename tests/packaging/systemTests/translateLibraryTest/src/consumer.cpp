#include "consumer.hpp"
#include "check_variables.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main() {
	if (!std::filesystem::exists(getExpectedOutFile())) {
		std::cerr << "Output file does not exist: " << getExpectedOutFile()
		          << '\n';
		return 1;
	}

	if (!std::filesystem::exists(getExpectedCombinedHeader())) {
		std::cerr << "Combined header does not exist: "
		          << getExpectedCombinedHeader() << '\n';
		return 1;
	}

	// Get contents of combined header
	std::ifstream combinedHeader(getExpectedCombinedHeader());
	std::string line;
	std::vector<std::string> lines;
	while (std::getline(combinedHeader, line)) {
		if (!line.empty()) {
			lines.push_back(line);
		}
	}

	auto publicHeaders = getPublicHeaders();
	if (publicHeaders.size() != lines.size()) {
		std::cerr
		    << "Number of public headers is not the same as numbers of non-blank lines in the combined header. The lines in the combined header:"
		    << '\n';
		for (auto const& line : lines) {
			std::cerr << line << '\n';
		}

		std::cerr << "All the public headers:" << '\n';
		for (auto const& publicHeader : publicHeaders) {
			std::cerr << publicHeader << '\n';
		}
		return 1;
	}

	for (auto const& header : publicHeaders) {
		std::string expecedInclude =
		    std::string("#include <") + std::string(header) + std::string(">");
		if (auto matchedLine =
		        std::find(lines.begin(), lines.end(), expecedInclude);
		    matchedLine == lines.end()) {
			std::cerr
			    << "Expected include did not appear in the combined header: \n"
			    << expecedInclude << '\n';

			std::cerr << "The lines in the combined header: " << '\n';
			for (auto const& line : lines) {
				std::cerr << line << '\n';
			}
			return 1;
		}
	}

	return 0;
}
