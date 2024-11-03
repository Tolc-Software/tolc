#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Write to file functions", "[functions]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

int sayTen() {
	return 10;
}

std::string giveBack(std::string const& s) {
	return s;
}

namespace MyNamespace {
	int add(int x, int y) {
		return x + y;
	}
	namespace Nested {
		int increase(int x) {
			return x + 1;
		}
	}
}
)";

  auto jsTestCode = R"(
expect(m.sayTen()).toBe(10);

expect(m.giveBack("hello")).toBe("hello");

// Nested functions are under their respective namespace
expect(m.MyNamespace.add(1, 2)).toBe(3);
expect(m.MyNamespace.Nested.increase(2)).toBe(3);
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Functions");
}
