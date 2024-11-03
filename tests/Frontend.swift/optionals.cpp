#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::optionals", "[optionals]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <optional>
#include <string>

std::string
answer(std::optional<std::string> const& question) {
  if (question) {
    return "Please be more specific.";
  }
  return "That's no question!";
}

)";

  auto objcTestCode = R"(
// std::optional is either the value or nil
NSString* answer = [m answer:@"How do I take over the world?"];
assert([answer isEqualToString:@"Please be more specific."]);

// nil is the equivalent of std::nullopt on the C++ side
NSString* noAnswer = [m answer:nil];
assert([noAnswer isEqualToString:@"That's no question!"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::optional");
}
