#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::maps", "[maps]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <map>
#include <string>

class MyClass {
public:
	explicit MyClass(std::map<std::string, int> s) : m_s(s) {}

	std::map<std::string, int> getS() { return m_s; }

	std::string getValue(std::map<int, std::string> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return it->second;
		}
		return "";
	}

private:
	std::map<std::string, int> m_s;
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::map translates to a normal dictionary in python
my_map = {{"hi": 4, "ho": 5}}
c = {moduleName}.MyClass(my_map)
self.assertEqual(c.getS(), my_map)

# The maps are typed on the C++ side
for incopatible_map in [{{"key": "value"}}, {{5: 2}}]:
    with self.assertRaises(TypeError) as error_context:
        c = {moduleName}.MyClass(incopatible_map)
        c.getValue(incopatible_map, 5)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible function arguments" in error_context.exception.args[0]
        or "incompatible constructor arguments"
        in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        str(incopatible_map) in error_context.exception.args[0],
        "Error msg does not mention the given arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::map");
}
