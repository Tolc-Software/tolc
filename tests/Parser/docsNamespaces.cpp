#include "TestUtil/contains.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

void checkDoc(IR::Namespace& parent,
              std::string name,
              std::string containsDoc) {
  auto& ns = TestUtil::findNamespace(parent, name);
  REQUIRE(TestUtil::contains(ns.m_documentation, containsDoc));
}

TEST_CASE("Can parse out comments", "[docsNamespaces]") {
  auto ns = TestUtil::parseString(R"(
// One line comment
namespace ns0 {}

/** Single multi line comment */
namespace ns1 {}

/**
* Multi
* line
* comment
*/
namespace ns2 {}

/**
Bare multi
Another line
*/
namespace ns3 {}

/*!
* Qt style
*/
namespace ns4 {}

/*****************************
* JavaDoc Style
* is
* boxy
****************************/
namespace ns5 {}

///
/// Triplets is a lifestyle
///
namespace ns6 {}

//!
//! This is one of the doxy styles
//!
namespace ns7 {}

namespace Nested {
	// Comment on nested
	namespace ns8 {}
};
)");
  checkDoc(ns, "ns0", "One line comment");

  checkDoc(ns, "ns1", "Single multi line comment");

  checkDoc(ns,
           "ns2",
           R"( Multi
 line
 comment)");

  checkDoc(ns,
           "ns3",
           R"(Bare multi
Another line)");

  checkDoc(ns, "ns4", "Qt style");

  checkDoc(ns,
           "ns5",
           R"(*************************
JavaDoc Style
is
boxy
*********************)");

  checkDoc(ns, "ns6", "Triplets is a lifestyle");

  checkDoc(ns, "ns7", "This is one of the doxy styles");

  auto& nested = TestUtil::findNamespace(ns, "Nested");
  REQUIRE(!nested.m_namespaces.empty());
  auto& ns8 = nested.m_namespaces.back();
  REQUIRE(ns8.m_documentation == "Comment on nested");
}
