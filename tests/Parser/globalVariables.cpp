#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Global variables", "[globalVariables]") {
  std::string code = R"(
int i;
namespace Nested {
  int i;
  int f;
  namespace EvenFurther {
    static int k;
  }
}
)";
  auto globalNS = TestUtil::parseString(code);
  auto& i = TestUtil::findVariable(globalNS, "i");
  TestUtil::compare(i.m_type, IR::BaseType::Int);
  REQUIRE(!i.m_isStatic);

  auto& nested = TestUtil::findNamespace(globalNS, "Nested");

  auto& nestedI = TestUtil::findVariable(nested, "i");
  TestUtil::compare(nestedI.m_type, IR::BaseType::Int);
  REQUIRE(!nestedI.m_isStatic);

  auto& f = TestUtil::findVariable(nested, "f");
  TestUtil::compare(f.m_type, IR::BaseType::Int);
  REQUIRE(!f.m_isStatic);

  auto& evenFurther = TestUtil::findNamespace(nested, "EvenFurther");

  auto& k = TestUtil::findVariable(evenFurther, "k");
  TestUtil::compare(k.m_type, IR::BaseType::Int);
  REQUIRE(k.m_isStatic);
}
