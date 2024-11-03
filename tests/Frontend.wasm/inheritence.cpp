#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/files.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Inheritence", "[inheritence]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

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

  auto jsTestCode = R"(
const fido = new m.Dog("Fido");

// Inherits public properties
expect(fido.name).toBe("Fido")

// But has its new functions
expect(fido.bark()).toBe("woof!")
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Simple inheritence");
}
