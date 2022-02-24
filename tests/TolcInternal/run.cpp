#include "TolcInternal/run.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <TestUtil/CommandLineInput.hpp>
#include <TestUtil/getTestFilesDirectory.hpp>
#include <catch2/catch.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <iostream>
#include <string>
#include <vector>

TestUtil::CommandLineInput getValidCLI(std::filesystem::path const& inputFile,
                                       std::vector<std::string> includes = {},
                                       std::string const& language = "python") {
	std::string input = fmt::format(
	    "tolc {language} --output testOutDir --module-name myModule --input {input}",
	    fmt::arg("language", language),
	    fmt::arg("input", inputFile.string()));

	for (auto const& include : includes) {
		input += " -I " + include;
	}

	input += " --no-analytics";

	std::cout << input << '\n';

	return TestUtil::CommandLineInput(input);
}

TEST_CASE("Base cases", "[run]") {
	for (std::string language : {"python", "wasm"}) {
		CAPTURE(language);
		auto cli = getValidCLI(
		    TestUtil::getTestFilesDirectory() / "base.hpp", {}, language);
		std::cout << "Got a valid cli" << '\n';
		auto exitCode = TolcInternal::run(cli.argc, cli.argv);
		REQUIRE(exitCode == 0);
	}
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
