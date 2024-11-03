#include "TestUtil/contains.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

void checkDoc(IR::Namespace& ns,
              std::string className,
              std::string containsDoc) {
  using TestUtil::contains;
  auto& strct = TestUtil::findStruct(ns, className);
  REQUIRE(contains(strct.m_documentation, containsDoc));
}

TEST_CASE("Can parse out class comments", "[docsClass]") {
  auto ns = TestUtil::parseString(R"(
// One line comment
class OneLiner {};

/** Single multi line comment */
class SingleMulti {};

/**
* Multi
* line
* comment
*/
class Multi {};

/**
Bare multi
Another line
*/
class BareMulti {};

/*!
* Qt style
*/
class QtStyle {};

/*******************************************************************************
* JavaDoc Style
* is
* boxy
******************************************************************************/
class JavaDoc {};

///
/// Triplets is a lifestyle
///
class Triplets {};

//!
//! This is one of the doxy styles
//!
class DoxyBang {};
)");
  checkDoc(ns, "OneLiner", "One line comment");

  checkDoc(ns, "SingleMulti", "Single multi line comment");

  checkDoc(ns,
           "Multi",
           R"( Multi
 line
 comment)");

  checkDoc(ns,
           "BareMulti",
           R"(Bare multi
Another line)");

  checkDoc(ns, "QtStyle", "Qt style");

  checkDoc(ns,
           "JavaDoc",
           R"(*******************************************************
JavaDoc Style
is
boxy
***************************************************)");

  checkDoc(ns, "Triplets", "Triplets is a lifestyle");

  checkDoc(ns, "DoxyBang", "This is one of the doxy styles");
}
