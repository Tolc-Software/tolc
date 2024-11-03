#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::multiset of base type", "[multisets]") {
  auto code = R"(
#include <set>
struct MyClass {
	std::multiset<int> m_d;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);

  REQUIRE(m_d.m_type.m_representation == "std::multiset<int>");
  auto multisetType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(multisetType != nullptr);

  REQUIRE(multisetType->m_container == IR::ContainerType::MultiSet);
  // NOTE: Second arg is the comparator, third is the allocator
  REQUIRE(multisetType->m_containedTypes.size() == 3);
  auto& type = multisetType->m_containedTypes.front();

  auto value = std::get_if<IR::Type::Value>(&type.m_type);
  REQUIRE(value != nullptr);
  REQUIRE(value->m_base == IR::BaseType::Int);
  REQUIRE(type.m_representation == "int");
}
