#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Write to file functions", "[functions]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <fstream>
#include <string>

void sayHello() {
	std::ofstream f("hello.txt");
	f << "Hello!";
	f.close();
}

void addYourOwn(std::string content) {
	std::ofstream f("hello.txt");
	f << content;
	f.close();
}

/**
* Documentation carries over
*/
int calculate() {
	return 5;
}

// Different documentation styles are supported
int missingArgumentsNaming(int, int i) {
	return i;
}

char firstLetter(std::string_view s) {
	return s[0];
}

int static getZero() {
	return 0;
}
)";

	auto pythonTestCode = fmt::format(R"(
{moduleName}.sayHello()
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), "Hello!")

content = "This is from python!"
{moduleName}.addYourOwn(content)
with open("hello.txt", "r") as f:
    self.assertEqual(f.readline(), content)

result = {moduleName}.calculate()
self.assertEqual(result, 5)
self.assertIn("Documentation carries over", {moduleName}.calculate.__doc__)

# Without naming variables is fine
result = {moduleName}.missingArgumentsNaming(2, 5)
self.assertEqual(result, 5)
self.assertIn("Different documentation styles are supported", \
  {moduleName}.missingArgumentsNaming.__doc__)

# Not possible to name any variables unless they are all known
with self.assertRaises(TypeError) as error_context:
  result = {moduleName}.missingArgumentsNaming(2, i=5)

# std::string_view works fine
result = {moduleName}.firstLetter("Hello")
self.assertEqual(result, "H")

# Static functions are just normal module functions in python
self.assertEqual({moduleName}.getZero(), 0)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Functions");
}
