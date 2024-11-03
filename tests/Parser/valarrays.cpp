#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("valarray of simple type", "[valarrays]") {
  auto code = R"(
#include <valarray>
struct MyClass {
	std::valarray<int> v;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& v =
      TestUtil::findMember(myClass, "v", TestUtil::AccessModifier::Public);
  TestUtil::compare(v.m_type, IR::ContainerType::Valarray);
}
