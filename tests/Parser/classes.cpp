#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Static and const member variables", "[classes]") {
  auto globalNS = TestUtil::parseString(R"(
#include <string>

class SimpleMember {
public:
	std::string myString;
};

class ConstMember {
public:
	const int i = 42;
};

class PrivateMember {
private:
	std::string str0;
	static std::string str1;
};

namespace MyLib {
	class Nested {
	public:
		double d = 4.3;
	};
}
)");
  auto& simpleMember = TestUtil::findStruct(globalNS, "SimpleMember");
  auto& myString = TestUtil::findMember(
      simpleMember, "myString", TestUtil::AccessModifier::Public);
  REQUIRE(!myString.m_isStatic);
  REQUIRE(!myString.m_type.m_isConst);

  auto& constMember = TestUtil::findStruct(globalNS, "ConstMember");
  auto& i =
      TestUtil::findMember(constMember, "i", TestUtil::AccessModifier::Public);
  REQUIRE(!i.m_isStatic);
  REQUIRE(i.m_type.m_isConst);

  auto& privateMember = TestUtil::findStruct(globalNS, "PrivateMember");
  auto& str0 = TestUtil::findMember(
      privateMember, "str0", TestUtil::AccessModifier::Private);
  auto& str1 = TestUtil::findMember(
      privateMember, "str1", TestUtil::AccessModifier::Private);
  REQUIRE(!str0.m_isStatic);
  REQUIRE(!str0.m_type.m_isConst);
  REQUIRE(str1.m_isStatic);
  REQUIRE(!str1.m_type.m_isConst);

  auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
  auto& nested = TestUtil::findStruct(myLib, "Nested");
  auto& d = TestUtil::findMember(nested, "d", TestUtil::AccessModifier::Public);
  REQUIRE(!d.m_isStatic);
  REQUIRE(!d.m_type.m_isConst);
}

TEST_CASE("Deeply nested classes", "[classes]") {
  auto code = R"(
#include <string>

namespace MyLib {

	class MyLibClass {
	};

	namespace Deeper {
		class MyDeeperClass {
		};
	}
}
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& myLib = globalNS.m_namespaces.back();
  REQUIRE(myLib.m_structs.size() == 1);
  REQUIRE(myLib.m_structs.back().m_name == "MyLibClass");
  REQUIRE(myLib.m_namespaces.size() == 1);
  auto& deeper = myLib.m_namespaces.back();
  REQUIRE(deeper.m_structs.size() == 1);
  REQUIRE(deeper.m_structs.back().m_name == "MyDeeperClass");
}

TEST_CASE("A class with a constructor", "[classes]") {
  auto globalNS = TestUtil::parseString(R"(
class Simple {
public:
	Simple();
};
		)");
  REQUIRE(globalNS.m_structs.size() == 1);
  auto& simple = globalNS.m_structs[0];
  REQUIRE(simple.m_name == "Simple");
  REQUIRE(simple.m_public.m_constructors.size() == 1);
  // NOTE: It has a default constructor manually written
  REQUIRE(!simple.m_hasImplicitDefaultConstructor);
  auto constructor = simple.m_public.m_constructors.back();
  REQUIRE(constructor.m_name == "Simple");
}

TEST_CASE("Finds a global class", "[classes]") {
  for (std::string structure : {"class", "struct"}) {
    auto globalNS = TestUtil::parseString(structure + " Simple {}; ");
    REQUIRE(globalNS.m_structs.size() == 1);
    auto& simple = globalNS.m_structs[0];
    REQUIRE(simple.m_hasImplicitDefaultConstructor);
    REQUIRE(simple.m_name == "Simple");
  }
}

TEST_CASE("Finds a class within a namespace", "[classes]") {
  auto globalNS = TestUtil::parseString(R"(
namespace NS {
	class Simple {};
}
		)");
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& ns = globalNS.m_namespaces[0];
  CHECK(ns.m_name == "NS");
  REQUIRE(ns.m_structs.size() == 1);
  auto& simple = ns.m_structs[0];
  REQUIRE(simple.m_name == "Simple");
}

TEST_CASE("Finds a struct within a struct", "[classes]") {
  auto globalNS = TestUtil::parseString(R"(
struct Outer {
	struct Inner {};
};
		)");
  REQUIRE(globalNS.m_structs.size() == 1);
  auto& outer = globalNS.m_structs[0];
  REQUIRE(outer.m_name == "Outer");
  REQUIRE(outer.m_public.m_structs.size() == 1);
  auto& inner = outer.m_public.m_structs[0];
  REQUIRE(inner.m_name == "Inner");
}
