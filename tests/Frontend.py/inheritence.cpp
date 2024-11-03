#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Inheritence", "[inheritence]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

struct Dog : public Pet {
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};
)";

	auto pythonTestCode = R"(
fido = m.Dog("Fido")

# Inherits public properties
self.assertEqual(fido.name, "Fido")

# But has its new functions
self.assertEqual(fido.bark(), "woof!")
)";

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Simple inheritence");
}
