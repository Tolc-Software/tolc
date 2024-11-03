#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::lists", "[lists]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>
#include <list>

std::list<std::string> getList() {
  return {"Linked", "list", "fun"};
}
)";

  auto objcTestCode = R"(
// std::list corresponds to NSArray
NSArray* words = [m getList];
assert([words count] == 3);

assert([[words objectAtIndex:0] isEqualToString:@"Linked"]);
assert([[words objectAtIndex:1] isEqualToString:@"list"]);
assert([[words objectAtIndex:2] isEqualToString:@"fun"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::list");
}
