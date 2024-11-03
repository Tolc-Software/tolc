#pragma once

#include <filesystem>
#include <fstream>
#include <limits>
#include <random>
#include <string>

namespace {
std::string getUniqueTempFilename(std::string const& ext = ".hpp") {
	// Use to build a random string
	std::random_device r;
	std::default_random_engine rEngine(r());
	std::uniform_int_distribution<> uniform(0, std::numeric_limits<int>::max());

	auto tempDir = std::filesystem::temp_directory_path();
	auto tempFile = tempDir / std::to_string(uniform(rEngine));
	tempFile += ext;
	while (std::filesystem::exists(tempFile)) {
		tempFile = tempDir / std::to_string(uniform(rEngine));
		tempFile += ext;
	}
	return tempFile.string();
}
}    // namespace

namespace TestUtil {

// A file that removes itself on destruction
// NOTE: Located in temp directory
//       Will write over any file with the same name in this directory
class TemporaryFile {
public:
	TemporaryFile(std::string const& filename, std::string const& content)
	    : m_filename(filename) {
		std::ofstream file(m_filename);
		file << content;
		file.close();
	}
	~TemporaryFile() {
		if (std::filesystem::exists(m_filename)) {
			std::filesystem::remove(m_filename);
		}
	}

	// Implicitly converts to the filename
	operator std::filesystem::path() const {
		return m_filename;
	}

private:
	std::filesystem::path m_filename;
};

// A directory that removes itself on destruction
// NOTE: Located in temp directory
class TemporaryDirectory {
public:
	// The path is appended to the temporary directory
	TemporaryDirectory(std::filesystem::path const& path) {
		// Use to build a random string
		std::random_device r;
		std::default_random_engine rEngine(r());
		std::uniform_int_distribution<> uniform(
		    0, std::numeric_limits<int>::max());
		auto tempDir = std::filesystem::temp_directory_path();

		// Ensure that the new stage does not exist before
		m_rootDirectory = tempDir / std::to_string(uniform(rEngine));
		while (std::filesystem::exists(m_rootDirectory)) {
			m_rootDirectory = tempDir / std::to_string(uniform(rEngine));
		}

		m_directory = m_rootDirectory / path;
		std::filesystem::create_directories(m_directory);
	}

	~TemporaryDirectory() {
		if (std::filesystem::exists(m_rootDirectory)) {
			std::filesystem::remove_all(m_rootDirectory);
		}
	}

	std::filesystem::path getRootDirectory() const {
		return m_rootDirectory;
	}

	// Implicitly converts to the directory
	operator std::filesystem::path() const {
		return m_directory;
	}

private:
	// This is the base of the temporary directory (what will get deleted)
	std::filesystem::path m_rootDirectory;
	// This is the full path to the user specified directory
	std::filesystem::path m_directory;
};

TemporaryFile writeToFile(std::string const& code,
                          std::string const& ext = ".hpp") {
	// TODO: Make the name unique
	//       so that more tests can use this at the same time
	TemporaryFile tf(getUniqueTempFilename(ext), code);
	return tf;
}

}    // namespace TestUtil
