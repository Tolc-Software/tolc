#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Simple valarrays", "[valarrays]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <valarray>

std::valarray<int> get() {
	return {1, 2, 3};
}
)";

	auto pythonTestCode = fmt::format(R"(
v = {moduleName}.get()
self.assertEqual(v, [1, 2, 3])
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::valarray");
}
