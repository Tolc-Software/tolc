#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

namespace TolcInternal {

int run(int argc, const char** argv) {
	if (auto maybeResult = CommandLine::parse(argc, argv)) {
		auto cliResult = maybeResult.value();
		// Check if user asked for --help
		if (cliResult.isHelp) {
			return 0;
		}

		if (auto maybeConfig = buildConfig(cliResult)) {
			auto config = maybeConfig.value();
			std::cout << "Got config with input: " << config.inputFile << '\n';

			return 0;
		}
	}

	return 1;
}

}    // namespace TolcInternal
