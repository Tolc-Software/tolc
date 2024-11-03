#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::vectors", "[vectors]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <algorithm>
#include <vector>

std::vector<int> f() {
  return {0, 1, 2};
}

bool allOf(std::vector<bool> const& conditions) {
  return std::all_of(
      conditions.begin(), conditions.end(),
      [](auto c) { return c; });
}

double sum(std::vector<double> const& numbers) {
  double sum = 0;
  for (double number : numbers) {
    sum += number;
  }
  return sum;
}

)";

  auto objcTestCode = R"(
// std::vector corresponds to NSArray
NSArray* v = [m f];
assert([v count] == 3);

// The vector contains {0, 1, 2}
assert([[v objectAtIndex:0] intValue] == 0);
assert([[v objectAtIndex:1] intValue] == 1);
assert([[v objectAtIndex:2] intValue] == 2);

// Sending NSArray into function works as well
NSArray* conditions = @[@(YES), @(YES), @(NO)];
assert([m allOf:conditions] == NO);

NSArray<NSNumber*>* toSum = @[@(1.1), @(2.2), @(3.3)];
assert([m sum:toSum] == 6.6);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::vector");
}
