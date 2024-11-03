#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::lists", "[lists]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <list>

class WithMember {
public:
	explicit WithMember(std::list<std::string> s) : m_s(s) {}

	std::list<std::string> getS() { return m_s; }

private:
	std::list<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::list<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::list translates to a normal array in python
my_array = ["hi", "ho"]
with_member = {moduleName}.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = {moduleName}.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3]), 6)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::list");
}
