#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Testing enums", "[enums]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
enum Unscoped {
	Under,
	Uboat,
};

enum class Scoped {
	Sacred,
	Snail,
};

class EnumTest {
public:
	explicit EnumTest(Scoped _s) : s(_s) {};

	Scoped s;
};

Unscoped f(Unscoped u) {
	return u;
}

namespace NS {
	// Documentation describing the enum
	enum class Deep {
		Double,
		Down,
	};
}

)";

	auto pythonTestCode = fmt::format(R"(
# C++11 enums work
scoped = {moduleName}.Scoped.Snail
enumTest = {moduleName}.EnumTest(scoped)
self.assertEqual(enumTest.s, scoped)

# Aswell as legacy enums
unscoped = {moduleName}.Unscoped.Uboat
u = {moduleName}.f(unscoped)
self.assertEqual(u, unscoped)

# Enums under namespaces are available under the corresponding submodule
deep = {moduleName}.NS.Deep.Down
self.assertNotEqual(deep, {moduleName}.NS.Deep.Double)

# Documentation carries over from C++
self.assertIn("Documentation describing the enum", {moduleName}.NS.Deep.__doc__)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Enums");
}
