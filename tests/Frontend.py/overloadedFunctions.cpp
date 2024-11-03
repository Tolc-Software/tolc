#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Overloaded functions", "[overloadedFunctions]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

// Overloaded free functions
std::string sayHello() {
	return "Hello!";
}
std::string sayHello(std::string to) {
	return std::string("Hello ") + to;
}

std::string safety() { return "Safe!"; }

class Overload {
public:
	// Overloaded constructor
	Overload() {};
	Overload(std::string) {};

	// Overloaded class functions
	std::string getStuff() { return "Stuff"; }
	std::string getStuff(std::string customStuff) { return customStuff; }

	std::string safety() { return "Safe!"; }
};

)";

	auto pythonTestCode = fmt::format(R"(
# Overloaded functions work the same as in C++
# Free function overload
self.assertEqual({moduleName}.sayHello(), "Hello!")
self.assertEqual({moduleName}.sayHello("to me!"), "Hello to me!")

# Class function overload
overload = {moduleName}.Overload()
overload = {moduleName}.Overload("Overloaded!")
self.assertEqual(overload.getStuff(), "Stuff")
self.assertEqual(overload.getStuff("My stuff"), "My stuff")

self.assertEqual(overload.safety(), "Safe!")
self.assertEqual(overload.safety(), {moduleName}.safety())
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Overloaded Functions");
}
