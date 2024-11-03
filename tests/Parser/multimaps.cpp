#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::multimap of base type", "[multimaps]") {
  auto code = R"(
#include <map>
struct MyClass {
	std::multimap<int, int> m_d;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);
  REQUIRE(m_d.m_type.m_representation == "std::multimap<int, int>");
  auto multimapType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(multimapType != nullptr);

  REQUIRE(multimapType->m_container == IR::ContainerType::MultiMap);
  // NOTE: Third arg is the comparator, fourth is the allocator
  REQUIRE(multimapType->m_containedTypes.size() == 4);
  auto& type = multimapType->m_containedTypes.front();

  auto value = std::get_if<IR::Type::Value>(&type.m_type);
  REQUIRE(value != nullptr);
  REQUIRE(value->m_base == IR::BaseType::Int);
  REQUIRE(type.m_representation == "int");
}
