#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::maps", "[maps]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <map>
#include <string>
#include <vector>

std::map<std::string, int> getThings() {
  return {{"Greetings", 5}};
}

std::map<std::string, std::vector<double>> getCities() {
  return {
  {"Stockholm",
    {59.33, 18.06}},
  {"San Francisco",
    {37.77, -122.43}}
  };
}
)";

  auto objcTestCode = R"(
// std::map translates to a NSDictionary
NSDictionary* dict = [m getThings];
assert([dict count] == 1);
NSNumber* n = [dict objectForKey:@"Greetings"];
assert(n != nil);
assert([n intValue] == 5);

// Nested containers work as well
NSDictionary* cities = [m getCities];
assert([cities count] == 2);
NSArray* stockholm = [cities objectForKey:@"Stockholm"];
assert(stockholm != nil);
assert([stockholm count] == 2);
assert([[stockholm objectAtIndex:0] doubleValue] == 59.33);
assert([[stockholm objectAtIndex:1] doubleValue] == 18.06);

NSArray* sanFrancisco = [cities objectForKey:@"San Francisco"];
assert(sanFrancisco != nil);
assert([sanFrancisco count] == 2);
assert([[sanFrancisco objectAtIndex:0] doubleValue] == 37.77);
assert([[sanFrancisco objectAtIndex:1] doubleValue] == -122.43);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::map");
}
