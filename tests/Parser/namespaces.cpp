#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <algorithm>

namespace {
void checkEmpty(IR::Namespace& ns) {
  CAPTURE(ns.m_name);

  CHECK(ns.m_functions.empty());
  CHECK(ns.m_structs.empty());
}
}    // namespace

TEST_CASE("Nested namespace", "[namespaces]") {
  auto code = R"(
#include <string>

namespace MyLib {

	int complexFunction() {
		return 5;
	}

	namespace Deeper {
		std::string meaningOfLife() {
			return "42";
		}
	}
}
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& myLib = globalNS.m_namespaces.back();
  REQUIRE(myLib.m_functions.size() == 1);
  REQUIRE(myLib.m_namespaces.size() == 1);
  REQUIRE(myLib.m_representation == "MyLib");
  auto& deeper = myLib.m_namespaces.back();
  REQUIRE(deeper.m_functions.size() == 1);
  REQUIRE(deeper.m_representation == "MyLib::Deeper");
}

TEST_CASE("Single namespace", "[namespaces]") {
  auto globalNS = TestUtil::parseString("namespace Test {}");
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& testNs = globalNS.m_namespaces[0];

  checkEmpty(testNs);

  REQUIRE(testNs.m_name == "Test");
}

TEST_CASE("Two namespaces", "[namespaces]") {
  auto globalNS = TestUtil::parseString(R"(
namespace Test0 {}
namespace Test1 {}
		)");
  auto& namespaces = globalNS.m_namespaces;
  REQUIRE(namespaces.size() == 2);
  for (auto& ns : namespaces) {
    checkEmpty(ns);

    for (auto name : {"Test0", "Test1"}) {
      CAPTURE(name);
      REQUIRE(std::any_of(
          namespaces.begin(), namespaces.end(), [name](auto const& ns) {
            return ns.m_name == name;
          }));
    }
  }
}

TEST_CASE("Two nested namespaces", "[namespaces]") {
  auto globalNS = TestUtil::parseString(R"(
namespace ParentNamespace {
	namespace ChildNamespace {}
}
		)");
  auto& namespaces = globalNS.m_namespaces;
  REQUIRE(namespaces.size() == 1);
  auto& ns = namespaces.back();
  checkEmpty(ns);

  // It has one child
  REQUIRE(ns.m_namespaces.size() == 1);
  auto& child = ns.m_namespaces.back();
  checkEmpty(ns);

  CHECK(ns.m_name == "ParentNamespace");
  CHECK(child.m_name == "ChildNamespace");
}

TEST_CASE("Two namespaces with the same name and level are the same",
          "[namespaces]") {
  auto globalNS = TestUtil::parseString(R"(
namespace Name {
}
namespace Name {
}
		)");
  auto& namespaces = globalNS.m_namespaces;
  REQUIRE(namespaces.size() == 1);
  auto& ns = namespaces.back();
  checkEmpty(ns);

  CHECK(ns.m_name == "Name");
}

TEST_CASE("Three nested namespaces", "[namespaces]") {
  auto globalNS = TestUtil::parseString(R"(
namespace ParentNamespace {
	namespace ChildNamespace {
		namespace GrandchildNamespace {}
	}
}
		)");
  auto& namespaces = globalNS.m_namespaces;
  REQUIRE(namespaces.size() == 1);
  auto& root = namespaces.back();
  checkEmpty(root);

  REQUIRE(root.m_namespaces.size() == 1);
  auto& child = root.m_namespaces.back();
  checkEmpty(child);

  REQUIRE(child.m_namespaces.size() == 1);
  auto& grandchild = child.m_namespaces.back();
  checkEmpty(grandchild);

  CHECK(root.m_name == "ParentNamespace");
  CHECK(child.m_name == "ChildNamespace");
  CHECK(grandchild.m_name == "GrandchildNamespace");
}

TEST_CASE("Same name but different qualified name", "[namespaces]") {
  auto globalNS = TestUtil::parseString(R"(
namespace A {
	namespace B {}
}
namespace B {}
		)");
  auto& namespaces = globalNS.m_namespaces;
  REQUIRE(namespaces.size() == 2);
  checkEmpty(namespaces[0]);
  checkEmpty(namespaces[1]);
  auto a = std::find_if(namespaces.begin(),
                        namespaces.end(),
                        [](auto const& ns) { return ns.m_name == "A"; });
  REQUIRE(a != namespaces.end());
  auto b = std::find_if(namespaces.begin(),
                        namespaces.end(),
                        [](auto const& ns) { return ns.m_name == "B"; });
  REQUIRE(a != namespaces.end());

  CHECK(a->m_namespaces.size() == 1);
  CHECK(b->m_namespaces.size() == 0);
  auto& aB = a->m_namespaces.back();
  checkEmpty(aB);

  CHECK(a->m_name == "A");
  CHECK(b->m_name == "B");
  CHECK(aB.m_name == "B");
}

TEST_CASE("Deeply nested namespace with correct representations",
          "[namespaces]") {
  auto code = R"(
#include <string>

namespace MyLib {

int complexFunction() {
	return 5;
}

namespace We {
	namespace Are {
		namespace Going {
			namespace Pretty {
				namespace Deep {
					std::string meaningOfLife() {
						return "42";
					}
				}
			}
		}
	}
}
}
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& myLib = globalNS.m_namespaces.back();
  REQUIRE(myLib.m_functions.size() == 1);
  REQUIRE(myLib.m_namespaces.size() == 1);
  REQUIRE(myLib.m_representation == "MyLib");

  auto& we = myLib.m_namespaces.back();
  REQUIRE(we.m_namespaces.size() == 1);
  REQUIRE(we.m_representation == "MyLib::We");

  auto& are = we.m_namespaces.back();
  REQUIRE(are.m_namespaces.size() == 1);
  REQUIRE(are.m_representation == "MyLib::We::Are");

  auto& going = are.m_namespaces.back();
  REQUIRE(going.m_namespaces.size() == 1);
  REQUIRE(going.m_representation == "MyLib::We::Are::Going");

  auto& pretty = going.m_namespaces.back();
  REQUIRE(pretty.m_namespaces.size() == 1);
  REQUIRE(pretty.m_representation == "MyLib::We::Are::Going::Pretty");

  auto& deep = pretty.m_namespaces.back();
  REQUIRE(deep.m_functions.size() == 1);
  REQUIRE(deep.m_representation == "MyLib::We::Are::Going::Pretty::Deep");

  auto& meaningOfLife = deep.m_functions.back();
  REQUIRE(meaningOfLife.m_representation ==
          "MyLib::We::Are::Going::Pretty::Deep::meaningOfLife");
}
