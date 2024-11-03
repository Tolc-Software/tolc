#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::deque of base type", "[deques]") {
  auto code = R"(
#include <deque>

struct MyClass {
	std::deque<int> m_d;
};

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);

  REQUIRE(m_d.m_type.m_representation == "std::deque<int>");
  auto dequeType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(dequeType != nullptr);

  REQUIRE(dequeType->m_container == IR::ContainerType::Deque);
  // NOTE: Second arg is the allocator
  REQUIRE(dequeType->m_containedTypes.size() == 2);
  auto& type = dequeType->m_containedTypes.front();

  if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
    REQUIRE(value->m_base == IR::BaseType::Int);
    REQUIRE(type.m_isConst == false);
    REQUIRE(type.m_isReference == false);
    REQUIRE(type.m_representation == "int");
  } else {
    INFO("The type is not a base type. Something must have gone wrong.");
    REQUIRE(false);
  }
}
