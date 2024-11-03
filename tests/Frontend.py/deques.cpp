#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::deques", "[deques]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <deque>

class WithMember {
public:
	explicit WithMember(std::deque<std::string> s) : m_s(s) {}

	std::deque<std::string> getS() { return m_s; }

private:
	std::deque<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::deque<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::deque translates to a normal array in python
my_array = ["hi", "ho"]
with_member = {moduleName}.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = {moduleName}.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3]), 6)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::deque");
}
