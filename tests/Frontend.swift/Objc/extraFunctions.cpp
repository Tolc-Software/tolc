#include "Objc/extraFunctions.hpp"
#include "Objc/Builders/enumBuilder.hpp"
#include "TestUtil/string.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Can build simple enums", "[extraFunctions]") {
  IR::Enum e;

  std::string enumName = "MyEnum";
  e.m_name = enumName;

  std::string moduleName = "Module";
  auto fullyQualifiedName = moduleName + "::" + enumName;
  e.m_representation = fullyQualifiedName;

  std::vector<std::string> values = {"A", "B"};
  std::vector<std::string> valuesObjc = {enumName + values[0],
                                         enumName + values[1]};
  e.m_values = values;

  e.m_isScoped = true;
  auto e2 = Objc::Builders::buildEnum(e, "m");
  auto [from, to] =
      Objc::Conversions::getEnumConversions(e, "m" + enumName, valuesObjc);
  // REQUIRE(from.first == "hi");

  // std::array<std::pair<std::string, std::string>, 2> getEnumConvertions(
  //     IR::Enum const& cppEnum,
  //     std::string const& objcEnum,
  //     std::vector<std::string> const& objcEnumValues);
}
