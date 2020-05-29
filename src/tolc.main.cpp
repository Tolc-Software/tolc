#include <Frontend/Python/frontend.hpp>
#include <Parser/Parse.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <lyra/lyra.hpp>
#include <sstream>
#include <string>

struct PythonConfig {
	static char const* name() {
		return "python";
	}
	std::filesystem::path inputFile;
	std::filesystem::path outputDirectory;

	PythonConfig(lyra::cli_parser& cli) {
		cli.add_argument(lyra::opt(inputFile, "input")
		                     .name("-i")
		                     .name("--input")
		                     .required()
		                     .help("The interface file to be parsed."));
		cli.add_argument(
		    lyra::opt(outputDirectory, "output")
		        .name("-o")
		        .name("--output")
		        .required()
		        .help(
		            "The output directory where the bindings will be stored."));
	}
};

int main(int argc, const char** argv) {
	auto cli = lyra::cli_parser();
	std::string language;
	bool show_help = false;
	cli.add_argument(lyra::help(show_help));
	cli.add_argument(lyra::arg(language, "language")
	                     .choices(PythonConfig::name())
	                     .required()
	                     .help("language to perform."));
	auto result = cli.parse({argc, argv});
	if (show_help) {
		std::cout << cli;
		return 0;
	}

	if (language == PythonConfig::name()) {
		PythonConfig p(cli);
		result = cli.parse({argc, argv});
		if (result) {
			std::cout << "Got input: " << p.inputFile << '\n';
			std::cout << "Got output: " << p.outputDirectory << '\n';
			if (auto ns = Parser::parseFile(p.inputFile)) {
				std::string fileBasename;
				std::stringstream ss(p.inputFile);
				std::getline(ss, fileBasename, '.');
				ns->m_name = fileBasename;
				auto [file, bindings] =
				    Frontend::Python::createModule(ns.value());
				std::filesystem::create_directory(p.outputDirectory);
				std::cout << "Bindings are in file: "
				          << p.outputDirectory / file << '\n';
				std::ofstream out {p.outputDirectory / file};
				out << bindings;
			}
		}
	}
	if (!result) {
		std::cerr << result.errorMessage() << "\n";
	}
	return result ? 0 : 1;
}
