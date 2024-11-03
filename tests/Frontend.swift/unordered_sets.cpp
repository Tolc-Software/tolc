#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::unordered_sets", "[unordered_sets]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>
#include <unordered_set>

std::unordered_set<std::string> getLanguages() {
	return {"C++", "Objective-C"};
}
)";

  auto objcTestCode = R"(
// std::unordered_set corresponds to NSSet
NSSet* languages = [m getLanguages];
assert([languages count] == 2);
assert([languages containsObject:@"C++"]);
assert([languages containsObject:@"Objective-C"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::unordered_set");
}
