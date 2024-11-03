#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::pairs", "[pairs]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::pair<std::string, int> s) : m_s(s) {}

	std::pair<std::string, int> getS() { return m_s; }

private:
	std::pair<std::string, int> m_s;
};

class WithFunction {
public:
	int sum(std::pair<int, int> v) {
		return v.first + v.second;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
# Converts to a tuple, but is convertible from array aswell
my_array = ["hi", 4]
for t in [my_array, tuple(my_array)]:
    with_member = {moduleName}.MyClass(t)
    self.assertEqual(with_member.getS(), tuple(t))

with_function = {moduleName}.WithFunction()
self.assertEqual(with_function.sum((1, 2)), 3)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::pair");
}
