#pragma once

#include <filesystem>
#include <optional>
#include <string>

namespace CommandLine {
struct Config {
	// The language chosen to be parsed
	std::string m_language;
	std::filesystem::path m_inputFile;
	std::filesystem::path m_outputDirectory;
};
}
