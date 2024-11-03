#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("smart pointer of simple type", "[smartPointers]") {
  auto code = R"(
#include <memory>
struct MyClass {
	std::unique_ptr<int> u;
	std::shared_ptr<int> s;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& u =
      TestUtil::findMember(myClass, "u", TestUtil::AccessModifier::Public);
  TestUtil::compare(u.m_type, IR::ContainerType::UniquePtr);

  auto& s =
      TestUtil::findMember(myClass, "s", TestUtil::AccessModifier::Public);
  TestUtil::compare(s.m_type, IR::ContainerType::SharedPtr);
}
