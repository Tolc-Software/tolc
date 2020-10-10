#include "CommandLine/parse.hpp"
#include <TestUtil/CommandLineInput.hpp>
#include <boost/ut.hpp>

int main() {
	using namespace boost::ut;

	"Help is valid input"_test = [] {
		for (auto const* helpCall : {"tolc --help", "tolc python --help"}) {
			auto cli = TestUtil::CommandLineInput(helpCall);
			auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
			// Parsed correctly
			expect(maybeParsed.has_value());

			// But is empty
			auto parsed = maybeParsed.value();
			expect(parsed.inputFile.empty());
			expect(parsed.outputDirectory.empty());
			expect(parsed.language.empty());
			expect(parsed.includes.size() == 0_u);

			// And has help flag set
			expect(parsed.isHelp);
		}
	};

	"Python language is chosen"_test = [] {
		auto cli = TestUtil::CommandLineInput(
		    "tolc python -i input -o output -I include");
		auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
		expect(maybeParsed.has_value());

		auto parsed = maybeParsed.value();
		expect(parsed.inputFile == "input");
		expect(parsed.outputDirectory == "output");
		expect(parsed.language == "python");
		expect((parsed.includes.size() == 1_u) >> fatal);
		expect(parsed.includes[0] == "include");

		// And has not help flag set
		expect(!parsed.isHelp);
	};

	"Fails for invalid input"_test = [] {
		for (auto const* invalidInput : {"tolc python",
		                                 "tolc fakeLanguage",
		                                 "tolc python --output noInput",
		                                 "tolc python --input noOutput"}) {
			auto cli = TestUtil::CommandLineInput(invalidInput);
			auto maybeParsed = CommandLine::parse(cli.argc, cli.argv);
			// Parsed correctly
			expect(maybeParsed.has_value() == false);
		}
	};
}
