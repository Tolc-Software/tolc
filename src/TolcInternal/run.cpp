#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

namespace TolcInternal {

int run(int argc, const char** argv) {
	if (auto maybeResult = CommandLine::parse(argc, argv)) {
		if (auto maybeConfig = buildConfig(maybeResult.value())) {
			auto config = maybeConfig.value();
			std::cout << "Got config with input: " << config.inputFile << '\n';

			return 0;
		}
	}

	return 1;
}

}    // namespace TolcInternal
