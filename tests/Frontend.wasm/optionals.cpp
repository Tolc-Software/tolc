#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::optionals", "[optionals]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <optional>
#include <string>

class WithMember {
public:
	explicit WithMember(std::optional<std::string> s) : m_s(s) {}

	std::optional<std::string> getS() { return m_s; }

private:
	std::optional<std::string> m_s;
};

class WithFunction {
public:
	std::optional<int> getNullopt() {
		return std::nullopt;
	}
};

)";

  auto pythonTestCode = fmt::format(R"(
greeting = "hello"
withMember = {moduleName}.WithMember(greeting)
self.assertEqual(withMember.getS(), greeting)

withFunction = {moduleName}.WithFunction()
self.assertEqual(withFunction.getNullopt(), None)
)",
                                    fmt::arg("moduleName", moduleName));

  auto errorCode = stage.runEmbindTest(cppCode, pythonTestCode, moduleName);
  REQUIRE(errorCode == 0);
}
