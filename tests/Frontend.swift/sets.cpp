#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::sets", "[sets]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <set>
#include <string>

std::set<std::string> getLanguages() {
	return {"English", "Spanish"};
}
)";

  auto objcTestCode = R"(
// std::set corresponds to NSOrderedSet
NSOrderedSet* languages = [m getLanguages];
assert([languages count] == 2);
assert([languages containsObject:@"English"]);
assert([languages containsObject:@"Spanish"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::set");
}
