#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Global variables are converted", "[globalVariables]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string_view>

int const i = 0;
double const d = 55;
std::string_view const stringView = "Hello world";
const char* charPtr = "Hello world";

namespace MyNamespace {
	int const i = 5;
}
)";

  auto jsTestCode = R"(
expect(m.i).toBe(0);
expect(m.d).toBe(55);

// Global strings of type std::string_view and const char* are converted
// Globals of type std::string has an open issue:
//   https://github.com/emscripten-core/emscripten/issues/16275
expect(m.stringView).toBe("Hello world");
expect(m.charPtr).toBe("Hello world");

// Globals within namespaces work
expect(m.MyNamespace.i).toBe(5);
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Global Variables");
}
