#include "CommandLine/parse.hpp"
#include <TestUtil/CommandLineInput.hpp>
#include <catch2/catch.hpp>

TEST_CASE("Help is valid input", "[parse]") {
	for (auto const* helpCall : {"tolc --help", "tolc python --help"}) {
		auto cli = TestUtil::CommandLineInput(helpCall);
		auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
		// Parsed correctly
		REQUIRE(maybeParsed.has_value());

		// But is empty
		auto parsed = maybeParsed.value();
		REQUIRE(parsed.inputFile.empty());
		REQUIRE(parsed.outputDirectory.empty());
		REQUIRE(parsed.language.empty());
		REQUIRE(parsed.includes.size() == 0);

		// And has help flag set
		REQUIRE(parsed.isHelp);
	}
}

TEST_CASE("Python language is chosen", "[parse]") {
	auto cli = TestUtil::CommandLineInput(
	    "tolc python -i input -o output -m myModule -I include");
	auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
	REQUIRE(maybeParsed.has_value());

	auto parsed = maybeParsed.value();
	REQUIRE(parsed.inputFile == "input");
	REQUIRE(parsed.outputDirectory == "output");
	REQUIRE(parsed.language == "python");
	REQUIRE(parsed.moduleName == "myModule");
	REQUIRE(parsed.includes.size() == 1);
	REQUIRE(parsed.includes[0] == "include");

	// And has not help flag set
	REQUIRE(!parsed.isHelp);
}

TEST_CASE("Fails for invalid input", "[parse]") {
	for (auto const* invalidInput : {"tolc python",
	                                 "tolc fakeLanguage",
	                                 "tolc python --output noInput",
	                                 "tolc python --input noOutput"}) {
		auto cli = TestUtil::CommandLineInput(invalidInput);
		auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
		// Parsed correctly
		REQUIRE(maybeParsed.has_value() == false);
	}
};

TEST_CASE("Module name is required", "[parse]") {
	auto cli = TestUtil::CommandLineInput("tolc python -i input -o output");
	auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
	// Parsed correctly
	REQUIRE(maybeParsed.has_value() == false);
};
