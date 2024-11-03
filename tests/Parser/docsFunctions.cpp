#include "TestUtil/contains.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

void checkDoc(IR::Namespace& ns,
              std::string enumName,
              std::string containsDoc) {
  using TestUtil::contains;
  auto& strct = TestUtil::findFunction(ns, enumName);
  REQUIRE(contains(strct.m_documentation, containsDoc));
}

TEST_CASE("Can parse out comments", "[docsFunctions]") {
  auto ns = TestUtil::parseString(R"(
// One line comment
void f0();

/** Single multi line comment */
void f1();

/**
* Multi
* line
* comment
*/
void f2();

/**
Bare multi
Another line
*/
void f3();

/*!
* Qt style
*/
void f4();

/*******************************************************************************
* JavaDoc Style
* is
* boxy
******************************************************************************/
void f5();

///
/// Triplets is a lifestyle
///
void f6();

//!
//! This is one of the doxy styles
//!
void f7();

struct Hidden {
	// Comment on member
	void f8();
};
)");
  checkDoc(ns, "f0", "One line comment");

  checkDoc(ns, "f1", "Single multi line comment");

  checkDoc(ns,
           "f2",
           R"( Multi
 line
 comment)");

  checkDoc(ns,
           "f3",
           R"(Bare multi
Another line)");

  checkDoc(ns, "f4", "Qt style");

  checkDoc(ns,
           "f5",
           R"(*******************************************************
JavaDoc Style
is
boxy
***************************************************)");

  checkDoc(ns, "f6", "Triplets is a lifestyle");

  checkDoc(ns, "f7", "This is one of the doxy styles");

  auto& hidden = TestUtil::findStruct(ns, "Hidden");
  REQUIRE(!hidden.m_public.m_functions.empty());
  auto& f8 = hidden.m_public.m_functions.back();
  REQUIRE(f8.m_documentation == "Comment on member");
}
