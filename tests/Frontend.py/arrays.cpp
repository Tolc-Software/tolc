#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::arrays", "[arrays]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <array>
#include <string>

class WithMember {
public:
	explicit WithMember(std::array<std::string, 2> s) : m_s(s) {}

	std::array<std::string, 2> getS() { return m_s; }

private:
	std::array<std::string, 2> m_s;
};

class WithFunction {
public:
	int sum(std::array<int, 5> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};

)";

	auto pythonTestCode = fmt::format(R"(
# std::array translates to a normal array in python
my_array = ["hi", "ho"]
with_member = {moduleName}.WithMember(my_array)
self.assertEqual(with_member.getS(), my_array)

with_function = {moduleName}.WithFunction()
self.assertEqual(with_function.sum([1, 2, 3, 4, 5]), 15)

# It still corresponds to a fixed amount of elements
for incompatible_array in [["too many", "too many", "too many"], ["too few"]]:
    with self.assertRaises(TypeError) as error_context:
        with_member = m.WithMember(incompatible_array)

    self.assertEqual(len(error_context.exception.args), 1)
    self.assertTrue(
        "incompatible constructor arguments" in error_context.exception.args[0],
        "Error msg does not mention incompatible arguments: "
        + str(error_context.exception.args[0]),
    )
    self.assertTrue(
        "Invoked with: " + str(incompatible_array)
        in error_context.exception.args[0],
        "Error msg does not mention the given arguments: " + str(error_context.exception.args[0]),
    )
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::array");
}
