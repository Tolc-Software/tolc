#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Function with reference parameter", "[references]") {
  auto code = R"(
#include <string>

void f(std::string const& s);

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  REQUIRE(f.m_arguments.size() == 1);
  auto& s = f.m_arguments.back();
  REQUIRE(s.m_type.m_isReference);
  REQUIRE(s.m_type.m_isConst);
  REQUIRE(s.m_type.m_numPointers == 0);
}

TEST_CASE("Class with reference variables", "[references]") {
  auto code = R"(
#include <string>

struct MyClass {
	int i;
	double& d;
	const std::string& s;
};

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_structs.size() == 1);
  auto& myClass = globalNS.m_structs.back();
  REQUIRE(myClass.m_public.m_memberVariables.size() == 3);
  auto modifier = TestUtil::AccessModifier::Public;

  auto const& i = TestUtil::findMember(myClass, "i", modifier);
  REQUIRE(!i.m_type.m_isConst);
  REQUIRE(!i.m_type.m_isReference);

  auto const& d = TestUtil::findMember(myClass, "d", modifier);
  REQUIRE(!d.m_type.m_isConst);
  REQUIRE(d.m_type.m_isReference);

  auto const& s = TestUtil::findMember(myClass, "s", modifier);
  REQUIRE(s.m_type.m_isConst);
  REQUIRE(s.m_type.m_isReference);
}
