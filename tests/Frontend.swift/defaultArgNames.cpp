#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Function arguments with no name", "[defaultArgNames]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(

int getLife(int) {
	return 42;
}

int getStuff(int, int, int) {
	return 42;
}

)";

  auto objcTestCode = R"(
// Single argument without a name
assert([m getLife:0] == 42);

// Multiple arguments without a name
// is named as [arg0, arg1, ...]
assert([m getStuff:0 arg1:5 arg2:10] == 42);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);
}
