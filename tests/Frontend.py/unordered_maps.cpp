#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::unordered_maps", "[unordered_maps]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>
#include <unordered_map>

class MyClass {
public:
	explicit MyClass(std::unordered_map<std::string, int> s) : m_s(s) {}

	std::unordered_map<std::string, int> getS() { return m_s; }

	std::string getValue(std::unordered_map<int, std::string> const& m, int key) {
		auto it = m.find(key);
		if (it != m.end()) {
			return it->second;
		}
		return "";
	}

private:
	std::unordered_map<std::string, int> m_s;
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::unordered_map translates to a normal dictionary in python
myunordered_map = {{"hi": 4, "ho": 5}}
c = {moduleName}.MyClass(myunordered_map)
self.assertEqual(c.getS(), myunordered_map)

# Test unordered_map of the wrong type
for incompatible_map in [{{"key": "value"}}, {{5: 2}}]:
    with self.assertRaises(TypeError) as error_context:
        c = {moduleName}.MyClass(incompatible_map)
        c.getValue(incompatible_map, 5)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible function arguments" in error_context.exception.args[0]
        or "incompatible constructor arguments"
        in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: \n\t"
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        str(incompatible_map) in error_context.exception.args[0],
        "Error msg does not mention the given arguments: \n\t"
        + str(error_context.exception.args[0]),
    )

)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::unordered_map");
}
