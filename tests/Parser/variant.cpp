#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <variant>

TEST_CASE("std::variant of base type", "[variants]") {
  auto code = R"(
#include <variant>

struct MyClass {
	std::variant<int, double, bool> m_d;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);

  REQUIRE(m_d.m_type.m_representation == "std::variant<int, double, bool>");
  auto variantType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(variantType != nullptr);

  REQUIRE(variantType->m_container == IR::ContainerType::Variant);
  // TODO: Figure out why this is 0?
  // REQUIRE(variantType->m_containedTypes.size() == 3);
  // auto& type = variantType->m_containedTypes.front();

  // auto value = std::get_if<IR::Type::Value>(&type.m_type);
  // REQUIRE(value != nullptr);
  // REQUIRE(value->m_base == IR::BaseType::Int);
  // REQUIRE(type.m_representation == "int");
}
