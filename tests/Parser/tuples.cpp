#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::tuple", "[tuples]") {
  auto code = R"(
#include <string>
#include <tuple>

class MyClass {
public:
	explicit MyClass(std::tuple<std::string, int> s) : m_t(s) {}

	auto getS() { return m_t; }

	std::tuple<std::string, int> m_t;
};

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_t =
      TestUtil::findMember(myClass, "m_t", TestUtil::AccessModifier::Public);
  REQUIRE(m_t.m_type.m_isConst == false);
  REQUIRE(m_t.m_type.m_representation == "std::tuple<std::string, int>");
  auto tupleType = std::get_if<IR::Type::Container>(&m_t.m_type.m_type);
  REQUIRE(tupleType != nullptr);

  REQUIRE(tupleType->m_container == IR::ContainerType::Tuple);
  REQUIRE(tupleType->m_containedTypes.size() == 2);
}
