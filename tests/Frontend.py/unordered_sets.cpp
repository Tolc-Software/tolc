#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::unordered_sets", "[unordered_sets]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <unordered_set>

class MyClass {
public:
	explicit MyClass(std::unordered_set<std::string> s) : m_s(s) {}

	std::unordered_set<std::string> getS() { return m_s; }

	int getValue(std::unordered_set<int> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return *it;
		}
		return -1;
	}

private:
	std::unordered_set<std::string> m_s;
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::unordered_set translates to a normal array or a set in python
my_unordered_set = {{"hi", "this is a unordered_set"}}
c = {moduleName}.MyClass(my_unordered_set)
self.assertEqual(c.getS(), my_unordered_set)

self.assertEqual(c.getValue({{1, 2, 3}}, 3), 3)
self.assertEqual(c.getValue({{1, 2, 3}}, 4), -1)

# Test unordered_set of the wrong type
for incompatible_set in [{{"key": "value"}}, (5, 2)]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incompatible_set)
        c.getValue(incompatible_set, 5)

self.assertEqual(len(error_context.exception.args), 1)
self.assertTrue(
    "incompatible function arguments" in error_context.exception.args[0]
    or "incompatible constructor arguments" in error_context.exception.args[0],
    "Error msg does not mention incompatible arguments: \n\t"
    + str(error_context.exception.args[0]),
)
self.assertTrue(
    str(incompatible_set) in error_context.exception.args[0],
    "Error msg does not mention the given arguments: \n\t"
    + str(error_context.exception.args[0]),
)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::unordered_set");
}
