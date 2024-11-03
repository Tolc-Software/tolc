#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::priority_queue of base type", "[priority_queues]") {
  auto code = R"(
#include <queue>
struct MyClass {
	std::priority_queue<int> m_d;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);

  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_d =
      TestUtil::findMember(myClass, "m_d", TestUtil::AccessModifier::Public);

  REQUIRE(m_d.m_type.m_representation == "std::priority_queue<int>");
  auto queueType = std::get_if<IR::Type::Container>(&m_d.m_type.m_type);
  REQUIRE(queueType != nullptr);

  REQUIRE(queueType->m_container == IR::ContainerType::PriorityQueue);
  // NOTE: Second arg is the internal container, third is the comparator
  REQUIRE(queueType->m_containedTypes.size() == 3);
  auto& type = queueType->m_containedTypes.front();

  auto value = std::get_if<IR::Type::Value>(&type.m_type);
  REQUIRE(value != nullptr);
  REQUIRE(value->m_base == IR::BaseType::Int);
  REQUIRE(type.m_representation == "int");
}
