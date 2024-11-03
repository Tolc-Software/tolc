#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::sets", "[sets]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <set>
#include <string>

class MyClass {
public:
	explicit MyClass(std::set<std::string> s) : m_s(s) {}

	std::set<std::string> getS() { return m_s; }

	int getValue(std::set<int> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return *it;
		}
		return -1;
	}

private:
	std::set<std::string> m_s;
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::set translates to a normal array or a set in python
mySet = {{"hi", "this is a set"}}
c = {moduleName}.MyClass(mySet)
self.assertEqual(c.getS(), mySet)

self.assertEqual(c.getValue({{1, 2, 3}}, 3), 3)
self.assertEqual(c.getValue({{1, 2, 3}}, 4), -1)

# Test set of the wrong type
for incompatibleset in [{{"key": "value"}}, (5, 2)]:
    with self.assertRaises(TypeError) as error_context:
        c = m.MyClass(incompatibleset)
        c.getValue(incompatibleset, 5)

self.assertEqual(len(error_context.exception.args), 1)
self.assertTrue(
    "incompatible function arguments" in error_context.exception.args[0]
    or "incompatible constructor arguments" in error_context.exception.args[0],
    "Error msg does not mention incompatible arguments: \n\t"
    + str(error_context.exception.args[0]),
)
self.assertTrue(
    str(incompatibleset) in error_context.exception.args[0],
    "Error msg does not mention the given arguments: \n\t"
    + str(error_context.exception.args[0]),
)


)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::set");
}
