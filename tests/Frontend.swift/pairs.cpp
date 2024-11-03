#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::pairs", "[pairs]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

class Greeter {
public:
  explicit Greeter(std::pair<std::string, int> greetings)
    : m_greetings(greetings) {}

  std::pair<std::string, int> getGreetings() {
    return m_greetings;
  }

  std::string joinGreetings() {
    std::string joined;
    for (int i = 0; i < m_greetings.second; ++i) {
      joined += m_greetings.first;
    }
    return joined;
  }

private:
  std::pair<std::string, int> m_greetings;
};
)";

  auto objcTestCode = R"(
// std::pair corresponds to a NSArray
// with two values
NSArray* greetings = [NSArray
  arrayWithObjects:@"Hey ", @(3), nil];
assert([greetings count] == 2);

// Sending a pair to a function
mGreeter* g = [[mGreeter alloc]
  initWithPairStringInt:greetings];

// Joining the greetings 3 times
NSString* joined = [g joinGreetings];
assert([joined isEqualToString:@"Hey Hey Hey "]);

// Error handling
@try {
  // Sending an array with size != 2
  NSArray* tooManyArgs =
    [greetings arrayByAddingObject:@"Oh no"];
  mGreeter* boom = [[mGreeter alloc]
    initWithPairStringInt:tooManyArgs];
  // Should throw exception before
  assert(NO);
} @catch(NSException* error) {
  assert([[error name] isEqualToString:@"TypeException"]);
  NSString* reason =
    @"The array passed does not match the number of types in a pair. Expected: 2, Got: 3.";
  assert([[error reason] isEqualToString:reason]);
}
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::pair");
}
