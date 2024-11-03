#include "TestUtil/contains.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

void checkDoc(IR::Namespace& ns,
              std::string variableName,
              std::string containsDoc) {
  using TestUtil::contains;
  auto& v = TestUtil::findVariable(ns, variableName);
  REQUIRE(contains(v.m_documentation, containsDoc));
}

TEST_CASE("Can parse out comments", "[docsVariables]") {
  auto ns = TestUtil::parseString(R"(
// One line comment
int i;

/** Single multi line comment */
int j;

/**
* Multi
* line
* comment
*/
int k;

/**
Bare multi
Another line
*/
int l;

/*!
* Qt style
*/
int m;

/*******************************************************************************
* JavaDoc Style
* is
* boxy
******************************************************************************/
int n;

///
/// Triplets is a lifestyle
///
int o;

//!
//! This is one of the doxy styles
//!
int p;

struct Hidden {
	// Comment on member
	int i;
};
)");
  checkDoc(ns, "i", "One line comment");

  checkDoc(ns, "j", "Single multi line comment");

  checkDoc(ns,
           "k",
           R"( Multi
 line
 comment)");

  checkDoc(ns,
           "l",
           R"(Bare multi
Another line)");

  checkDoc(ns, "m", "Qt style");

  checkDoc(ns,
           "n",
           R"(*******************************************************
JavaDoc Style
is
boxy
***************************************************)");

  checkDoc(ns, "o", "Triplets is a lifestyle");

  checkDoc(ns, "p", "This is one of the doxy styles");

  auto& hidden = TestUtil::findStruct(ns, "Hidden");
  REQUIRE(!hidden.m_public.m_memberVariables.empty());
  auto& i = hidden.m_public.m_memberVariables.back();
  REQUIRE(i.m_documentation == "Comment on member");
}
