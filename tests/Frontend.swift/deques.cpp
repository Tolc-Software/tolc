#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::deques", "[deques]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>
#include <deque>

std::deque<std::string>
surround(std::deque<std::string> d,
         std::string const& message) {
  d.push_front(message);
  d.push_back(message);
  return d;
}
)";

  auto objcTestCode = R"(
// std::deque corresponds to NSArray
NSArray* myDeque = @[@"middle"];
NSArray* surroundedDeque =
  [m surround:myDeque message:@"surrounded"];
assert([surroundedDeque count] == 3);

assert([[surroundedDeque objectAtIndex:0]
  isEqualToString:@"surrounded"]);

assert([[surroundedDeque objectAtIndex:1]
  isEqualToString:@"middle"]);

assert([[surroundedDeque objectAtIndex:2]
  isEqualToString:@"surrounded"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::deque");
}
