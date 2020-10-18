#include "TolcInternal/run.hpp"
#include <TestUtil/CommandLineInput.hpp>
#include <TestUtil/getTestFilesDirectory.hpp>
#include <boost/ut.hpp>
#include <filesystem>
#include <string>
#include <vector>

TestUtil::CommandLineInput getValidCLI(std::filesystem::path const& inputFile,
                                       std::vector<std::string> includes = {}) {
	std::string input =
	    "tolc python --output testOutDir --module-name myModule --input ";
	input += inputFile;

	for (auto const& include : includes) {
		input += " -I " + include;
	}

	return TestUtil::CommandLineInput(input);
}

int main() {
	using namespace boost::ut;

	"Base cases"_test = [] {
		auto cli = getValidCLI(TestUtil::getTestFilesDirectory() / "base.hpp");
		auto exitCode = TolcInternal::run(cli.argc, cli.argv);
		expect(exitCode == 0_i);
	};

	"Standard library includes"_test = [] {
		auto cli = getValidCLI(TestUtil::getTestFilesDirectory() / "std.hpp");
		auto exitCode = TolcInternal::run(cli.argc, cli.argv);
		expect(exitCode == 0_i);
	};

	"User provided includes"_test = [] {
		auto testDir = TestUtil::getTestFilesDirectory();
		auto cli = getValidCLI(testDir / "include.hpp", {testDir / "include"});
		auto exitCode = TolcInternal::run(cli.argc, cli.argv);
		expect(exitCode == 0_i);
	};
}
