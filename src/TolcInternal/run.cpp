#include "TolcInternal/run.hpp"
#include "CommandLine/parse.hpp"
#include "TolcInternal/buildConfig.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

namespace TolcInternal {

int run(int argc, const char** argv) {
	if (auto maybeResult = CommandLine::parse(argc, argv)) {
		auto result = maybeResult.value();
		if (auto maybeConfig = buildConfig(result)) {
			auto config = maybeConfig.value();
			std::cout << "Got config with input: " << config.inputFile << '\n';
		}
	}

	return 1;
}

}    // namespace TolcInternal
