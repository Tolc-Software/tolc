#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Classes", "[documentationStyles]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
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

/*****************************
* JavaDoc Style
* is
* boxy
****************************/
class JavaDoc {};

///
/// Triplets is a commitment
///
class Triplets {};

//!
//! This is one of the doxy styles
//!
class DoxyBang {};
)";

	auto pythonTestCode = fmt::format(R"(
# These types of documentations are supported for:
#   Classes
#   Member variables
#   Enums
#   Functions

self.assertIn("One line comment", {moduleName}.OneLiner.__doc__)

self.assertIn("Single multi line", {moduleName}.SingleMulti.__doc__)

self.assertIn("Multi", {moduleName}.Multi.__doc__)

self.assertIn("Bare multi", {moduleName}.BareMulti.__doc__)

self.assertIn("Qt style", {moduleName}.QtStyle.__doc__)

self.assertIn("JavaDoc Style", {moduleName}.JavaDoc.__doc__)

self.assertIn("Triplets", {moduleName}.Triplets.__doc__)

self.assertIn("one of the doxy styles", {moduleName}.DoxyBang.__doc__)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Documentation Styles");
}
