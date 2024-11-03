#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Write to file functions", "[functions]") {
  auto cppCode = R"(
#include <filesystem>
#include <string>

int meaningOfLife() {
	return 42;
}

std::string sayHello(std::string const& name) {
	return "Hello " + name;
}

std::filesystem::path getPath() {
	return std::filesystem::path("/path/to/stuff.hpp");
}

namespace Inner {
	double pi() {
		return 3.14;
	}
}
)";

  [[maybe_unused]] auto objcTestCode = R"(
// Global functions gets added to
// a purely static class with
// the name of the library
assert([m meaningOfLife] == 42);

// Strings can be used
assert([[m sayHello:@"Tolc"] isEqualToString:@"Hello Tolc"]);

// Aswell as filesystem paths
assert([[m getPath] isEqualToString:@"/path/to/stuff.hpp"]);

// Functions within namespaces
// are available with the
// namespaces names merged
assert([mInner pi] == 3.14);
)";

  auto swiftTestCode = R"(
// Global functions gets added to
// a purely static class with
// the name of the library
assert(m.meaningOfLife() == 42)
)";

  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);
  stage.keepAliveAfterTest();
  // REQUIRE(stage.runTest(cppCode, objcTestCode, "objc") == 0);
  REQUIRE(stage.runTest(cppCode, swiftTestCode, "swift") == 0);

  stage.exportAsExample("Functions");
}
