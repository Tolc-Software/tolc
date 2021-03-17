#include "TolcInternal/getExecutableDirectoryPath.hpp"
#include <catch2/catch.hpp>
#include <filesystem>

TEST_CASE(
    "getExecutableDirectoryPath() returns the full path to this test executable",
    "[getExecutableDirectoryPath]") {
	auto exePath = TolcInternal::getExecutableDirectoryPath();
	// We should get something
	REQUIRE(!std::filesystem::is_empty(exePath));
	// It has to be the full path
	REQUIRE(exePath.is_absolute());
	// It should be a directory
	REQUIRE(std::filesystem::is_directory(exePath));
}
