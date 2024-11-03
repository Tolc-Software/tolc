#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::vectors", "[vectors]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <vector>

std::vector<int> getData() {
	return {0, 1, 2};
}

)";

  auto jsTestCode = R"(
const data = m.getData();

expect(data.size()).toBe(3);

for (let i = 0; i < data.size(); i++) {
    expect(data.get(i)).toBe(i);
}

data.push_back(3);

expect(data.size()).toBe(4);

expect(data.get(3)).toBe(3);
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::vector");
}
