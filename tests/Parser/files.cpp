#include "TestUtil/files.hpp"
#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Simple namespace", "[files]") {
  auto file = TestUtil::writeToFile(R"(
namespace Simple {}
)");
  auto globalNS = TestUtil::parseFile(file);
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& testNs = globalNS.m_namespaces[0];

  REQUIRE(testNs.m_name == "Simple");
}

TEST_CASE("Simple struct", "[files]") {
  auto file = TestUtil::writeToFile(R"(
class Simple {};
)");
  auto globalNS = TestUtil::parseFile(file);
  REQUIRE(globalNS.m_structs.size() == 1);
  auto& simple = globalNS.m_structs[0];
  REQUIRE(simple.m_name == "Simple");
}

TEST_CASE("Simple function", "[files]") {
  auto file = TestUtil::writeToFile(R"(
void fun();
)");
  auto globalNS = TestUtil::parseFile(file);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto fun = globalNS.m_functions[0];
  CHECK(fun.m_name == "fun");
  CHECK(fun.m_arguments.size() == 0);
  TestUtil::compare(fun.m_returnType, IR::BaseType::Void);
}

TEST_CASE("Simple field", "[files]") {
  auto file = TestUtil::writeToFile(R"(
class MyClass {
	int i;
};
		)");
  auto globalNS = TestUtil::parseFile(file);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  auto& i =
      TestUtil::findMember(myClass, "i", TestUtil::AccessModifier::Private);

  CHECK(i.m_name == "i");
  REQUIRE(!i.m_type.m_isConst);
  TestUtil::compare(i.m_type, IR::BaseType::Int);
}
