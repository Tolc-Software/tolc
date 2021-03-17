#include "TolcInternal/buildConfig.hpp"
#include "CommandLine/parse.hpp"
#include <TestUtil/getTestFilesDirectory.hpp>
#include <catch2/catch.hpp>

CommandLine::CLIResult buildMockCLI() {
	CommandLine::CLIResult cli;
	// "Borrow" one of the test files
	// Just need a file that exists
	cli.inputFile = (TestUtil::getTestFilesDirectory() / "base.hpp").string();
	cli.outputDirectory = "out";
	cli.language = "python";
	cli.moduleName = "myModule";
	return cli;
}

TEST_CASE("Translate valid cli to valid config", "[buildConfig]") {
	auto config = TolcInternal::buildConfig(buildMockCLI());
	REQUIRE(config.has_value());
};

TEST_CASE("Config has 'refined' the cli input", "[buildConfig]") {
	auto cli = buildMockCLI();
	// Make sure it is python so we can check enum
	cli.language = "python";
	auto maybeConfig = TolcInternal::buildConfig(cli);
	REQUIRE(maybeConfig.has_value());

	auto config = maybeConfig.value();
	REQUIRE(config.language == TolcInternal::Config::Language::Python);

	// Check string -> filesystem::path
	REQUIRE(config.inputFile == cli.inputFile);
	// Check string -> filesystem::path
	REQUIRE(config.outputDirectory == cli.outputDirectory);

	REQUIRE(!config.parserConfig.m_systemIncludes.empty());
};
