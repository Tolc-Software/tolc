#include "TolcInternal/buildConfig.hpp"
#include "CommandLine/parse.hpp"
#include <boost/ut.hpp>

CommandLine::CLIResult buildMockCLI() {
	CommandLine::CLIResult cli;
	// NOTE: This is a valid file since it is the name of the test executable
	cli.inputFile = "buildConfig";
	cli.outputDirectory = "out";
	cli.language = "python";
	cli.moduleName = "myModule";
	return cli;
}

int main() {
	using namespace boost::ut;

	"Translate valid cli to valid config"_test = [] {
		auto config = TolcInternal::buildConfig(buildMockCLI());
		expect(config.has_value() >> fatal);
	};

	"Config has 'refined' the cli input"_test = [] {
		auto cli = buildMockCLI();
		// Make sure it is python so we can check enum
		cli.language = "python";
		auto maybeConfig = TolcInternal::buildConfig(cli);
		expect(maybeConfig.has_value() >> fatal);

		auto config = maybeConfig.value();
		expect(config.language == TolcInternal::Config::Language::Python);

		// Check string -> filesystem::path
		expect(config.inputFile == cli.inputFile);
		// Check string -> filesystem::path
		expect(config.outputDirectory == cli.outputDirectory);

		expect(!config.parserConfig.m_systemIncludes.empty());
	};
}
