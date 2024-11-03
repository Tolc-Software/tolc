#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::list of base type", "[lists]") {
  auto code = R"(
#include <list>
struct MyClass {
	std::list<int> m_d;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);
  REQUIRE(m_d.m_type.m_representation == "std::list<int>");
  auto listType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(listType != nullptr);

  REQUIRE(listType->m_container == IR::ContainerType::List);
  // NOTE: Second arg is the allocator
  REQUIRE(listType->m_containedTypes.size() == 2);
  auto& type = listType->m_containedTypes.front();

  auto value = std::get_if<IR::Type::Value>(&type.m_type);
  REQUIRE(value != nullptr);
  REQUIRE(value->m_base == IR::BaseType::Int);
  REQUIRE(type.m_representation == "int");
}
