#include <CommandLine/Config.hpp>
#include <CommandLine/buildConfig.hpp>
#include <Frontend/Python/frontend.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace {}

int main(int argc, const char** argv) {
	if (auto maybeConfig = CommandLine::buildConfig(argc, argv)) {
		auto config = maybeConfig.value();
		if (auto ns = Parser::parseFile(config.m_inputFile)) {
			ns->m_name = config.m_inputFile.stem();
			auto [file, bindings] = Frontend::Python::createModule(ns.value());
			std::filesystem::create_directory(config.m_outputDirectory);
			std::cout << "Bindings are in file: "
			          << config.m_outputDirectory / file << '\n';
			std::ofstream out {config.m_outputDirectory / file};
			out << bindings;
			return EXIT_SUCCESS;
		}
	}

	return EXIT_FAILURE;
}
