#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::unordered_maps", "[unordered_maps]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>
#include <unordered_map>

std::unordered_map<std::string, int>
getUnordered() {
  return {{"Unordered", 1}};
}
)";

  auto objcTestCode = R"(
// std::unordered_map translates to a NSDictionary
NSDictionary* dict = [m getUnordered];
assert([dict count] == 1);
NSNumber* n = [dict objectForKey:@"Unordered"];
assert(n != nil);
assert([n intValue] == 1);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::unordered_map");
}
