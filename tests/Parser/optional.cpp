#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <variant>

TEST_CASE("std::optional of base type", "[optionals]") {
  auto code = R"(
#include <optional>

struct MyClass {
	std::optional<int> m_d;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);

  REQUIRE(m_d.m_type.m_representation == "std::optional<int>");
  auto optionalType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(optionalType != nullptr);

  REQUIRE(optionalType->m_container == IR::ContainerType::Optional);
  // NOTE: Second arg is the allocator
  REQUIRE(optionalType->m_containedTypes.size() == 1);
  auto& type = optionalType->m_containedTypes.front();

  auto value = std::get_if<IR::Type::Value>(&type.m_type);
  REQUIRE(value != nullptr);
  REQUIRE(value->m_base == IR::BaseType::Int);
  REQUIRE(type.m_representation == "int");
}
