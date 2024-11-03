#include "TestUtil/contains.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

void checkDoc(IR::Namespace& ns,
              std::string enumName,
              std::string containsDoc) {
  using TestUtil::contains;
  auto& e = TestUtil::findEnum(ns, enumName);
  REQUIRE(contains(e.m_documentation, containsDoc));
}

TEST_CASE("Can parse out comments", "[docsEnums]") {
  auto ns = TestUtil::parseString(R"(
// One line comment
enum class e0 {
	E,
};

/** Single multi line comment */
enum class e1 {
	E,
};

/**
* Multi
* line
* comment
*/
enum class e2 {
	E,
};

/**
Bare multi
Another line
*/
enum class e3 {
	E,
};

/*!
* Qt style
*/
enum class e4 {
	E,
};

/*******************************************************************************
* JavaDoc Style
* is
* boxy
******************************************************************************/
enum class e5 {
	E,
};

///
/// Triplets is a lifestyle
///
enum class e6 {
	E,
};

//!
//! This is one of the doxy styles
//!
enum class e7 {
	E,
};

struct Hidden {
	// Comment on member
	enum class e8 {
		E,
	};
};
)");
  checkDoc(ns, "e0", "One line comment");

  checkDoc(ns, "e1", "Single multi line comment");

  checkDoc(ns,
           "e2",
           R"( Multi
 line
 comment)");

  checkDoc(ns,
           "e3",
           R"(Bare multi
Another line)");

  checkDoc(ns, "e4", "Qt style");

  checkDoc(ns,
           "e5",
           R"(*******************************************************
JavaDoc Style
is
boxy
***************************************************)");

  checkDoc(ns, "e6", "Triplets is a lifestyle");

  checkDoc(ns, "e7", "This is one of the doxy styles");

  auto& hidden = TestUtil::findStruct(ns, "Hidden");
  REQUIRE(!hidden.m_public.m_enums.empty());
  auto& e8 = hidden.m_public.m_enums.back();
  REQUIRE(e8.m_documentation == "Comment on member");
}
