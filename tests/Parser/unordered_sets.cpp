#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::unordered_set of base type", "[unordered_sets]") {
  auto code =
      R"(
#include <unordered_set>

struct MyClass {
	std::unordered_set<int> m_v;
};

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_v =
      TestUtil::findMember(myClass, "m_v", TestUtil::AccessModifier::Public);
  REQUIRE(m_v.m_type.m_representation == "std::unordered_set<int>");
  auto arrayType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
  REQUIRE(arrayType != nullptr);

  REQUIRE(arrayType->m_container == IR::ContainerType::UnorderedSet);
  // class std::unordered_set<int, struct std::hash<int>, struct std::equal_to<int>, class std::allocator<int> >
  REQUIRE(arrayType->m_containedTypes.size() == 4);

  for (auto const& type : arrayType->m_containedTypes) {
    if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
      REQUIRE(value->m_base == IR::BaseType::Int);
      REQUIRE(type.m_isConst == false);
      REQUIRE(type.m_isReference == false);
      REQUIRE(type.m_representation == "int");
    } else if (auto container =
                   std::get_if<IR::Type::Container>(&type.m_type)) {
      REQUIRE(container->m_containedTypes.size() == 1);
      auto containerType = container->m_container;
      REQUIRE((containerType == IR::ContainerType::Hash ||
               containerType == IR::ContainerType::EqualTo ||
               containerType == IR::ContainerType::Allocator));
    } else {
      INFO(
          "The unordered_set does not contain expected templated values. Something must have gone wrong.");
      REQUIRE(false);
    }
  }
}
