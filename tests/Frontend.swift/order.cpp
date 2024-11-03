#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("The order should reflect the code and not IR", "[order]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <filesystem>
#include <string>

namespace Deep {
struct MeaningOfLife {
  int answer = 42;
};
}

// Global function using namespace nested data
int answer(Deep::MeaningOfLife m) {
	return m.answer;
}
)";

  auto objcTestCode = R"(
// Object defined in nested namespace
mDeepMeaningOfLife* meaning = [[mDeepMeaningOfLife alloc] init];

// Function in global namespace using nested class
// Since we are parsing breadth first, this function would end up
// before the class definition causing an error (unknown class).
// But since the function is using the class it is moved further down
assert([m answer:meaning] == 42);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);
}
