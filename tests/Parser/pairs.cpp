#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("std::pair", "[pairs]") {
  auto code = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::pair<std::string, int> s) : m_s(s) {}

	auto getS() { return m_s; }

	std::tuple<std::string, int> m_s;
};

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& m_t =
      TestUtil::findMember(myClass, "m_s", TestUtil::AccessModifier::Public);
  REQUIRE(m_t.m_type.m_isConst == false);
  auto pairType = std::get_if<IR::Type::Container>(&m_t.m_type.m_type);
  REQUIRE(pairType != nullptr);

  REQUIRE(pairType->m_containedTypes.size() == 2);
}
