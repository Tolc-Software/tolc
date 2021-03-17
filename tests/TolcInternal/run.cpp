#include "TolcInternal/run.hpp"
#include <TestUtil/CommandLineInput.hpp>
#include <TestUtil/getTestFilesDirectory.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <string>
#include <vector>

TestUtil::CommandLineInput getValidCLI(std::filesystem::path const& inputFile,
                                       std::vector<std::string> includes = {}) {
	std::string input =
	    "tolc python --output testOutDir --module-name myModule --input ";
	input += inputFile.string();

	for (auto const& include : includes) {
		input += " -I " + include;
	}

	return TestUtil::CommandLineInput(input);
}

TEST_CASE("Base cases", "[run]") {
	auto cli = getValidCLI(TestUtil::getTestFilesDirectory() / "base.hpp");
	auto exitCode = TolcInternal::run(cli.argc, cli.argv);
	REQUIRE(exitCode == 0);
};

TEST_CASE("Standard library includes", "[run]") {
	auto cli = getValidCLI(TestUtil::getTestFilesDirectory() / "std.hpp");
	auto exitCode = TolcInternal::run(cli.argc, cli.argv);
	REQUIRE(exitCode == 0);
};

TEST_CASE("User provided includes", "[run]") {
	auto testDir = TestUtil::getTestFilesDirectory();
	auto cli =
	    getValidCLI(testDir / "include.hpp", {(testDir / "include").string()});
	auto exitCode = TolcInternal::run(cli.argc, cli.argv);
	REQUIRE(exitCode == 0);
};
