#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("std::filesystem::path gets converted to pathlib.Path",
          "[filesystem_paths]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <filesystem>
#include <vector>

std::filesystem::path
takingPath(std::filesystem::path const& p) {
	return p;
}

std::filesystem::path
parent(std::filesystem::path const& p) {
	return p.parent_path();
}

std::filesystem::path
joinPaths(std::vector<std::filesystem::path> arrayToSum) {
	std::filesystem::path sum;
	for (auto f : arrayToSum) {
		sum /= f;
	}
	return sum;
}
)";

  auto objcTestCode = R"(
// std::filesystem::path corresponds to NSString
NSString* path = @"Hello/my/name/is/Tolc";

// Passing through a function
NSString* result = [m takingPath:path];
assert([result isEqualToString:path]);

NSString* parent = [m parent:path];
assert([parent isEqualToString:@"Hello/my/name/is"]);

NSArray* paths = @[@"to", @"the", @"heart"];
NSString* joined = [m joinPaths:paths];
assert([joined isEqualToString:@"to/the/heart"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::filesystem::path");
}
