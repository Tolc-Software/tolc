#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("std::filesystem::path gets converted to pathlib.Path",
          "[filesystem_paths]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <filesystem>
#include <vector>

std::filesystem::path takingPath(std::filesystem::path p) {
	return p;
}

std::string toString(std::filesystem::path p) {
	return p.string();
}

std::filesystem::path joinPaths(std::vector<std::filesystem::path> arrayToSum) {
	std::filesystem::path sum;
	for (auto f : arrayToSum) {
		sum /= f;
	}
	return sum;
}
)";

	auto pythonTestCode = fmt::format(R"(
# std::filesystem::path translates to pathlib.Path in python
from pathlib import Path

p0 = Path("Hello")
result0 = {moduleName}.takingPath(p0)
self.assertEqual(result0, p0)

p1 = Path("Something")
toString = {moduleName}.toString(p1)
self.assertEqual(toString, p1.name)

result1 = {moduleName}.joinPaths([p0, p1])
self.assertEqual(result1, p0 / p1)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::filesystem::path");
}
