#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Taking functions as arguments", "[functional]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <functional>
#include <vector>

double takingFunction(std::function<double(int)> callMe) {
	return callMe(5);
}

std::function<int(int)> returnFunction(const std::function<int(int)> &f) {
	return [f](int i) {
		return f(i) + 1;
	};
}

int accumulateArrayOfFunctions(std::vector<std::function<int()>> arrayToSum) {
	int sum = 0;
	for (auto f : arrayToSum) {
		sum += f();
	}
	return sum;
}
)";

	auto pythonTestCode = fmt::format(R"(
def callback(i):
  return i

# You can send a python function as a C++ callback
result0 = {moduleName}.takingFunction(callback)
self.assertEqual(result0, 5.0)

# Or in the other direction
inc_by_one = {moduleName}.returnFunction(callback)
self.assertEqual(inc_by_one(5), 6)

def fiver():
  return 5

# Or a vector of functions
result1 = {moduleName}.accumulateArrayOfFunctions([fiver, fiver])
self.assertEqual(result1, 10)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::function");
}
