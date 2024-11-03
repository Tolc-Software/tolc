#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::deques", "[deques]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>
#include <deque>

class WithMember {
public:
	explicit WithMember(std::deque<std::string> s) : m_s(s) {}

	std::deque<std::string> getS() { return m_s; }

private:
	std::deque<std::string> m_s;
};

class WithFunction {
public:
	int sum(std::deque<int> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};

)";

  auto pythonTestCode = fmt::format(R"(
myArray = ["hi", "ho"]
withMember = {moduleName}.WithMember(myArray)
self.assertEqual(withMember.getS(), myArray)

withFunction = {moduleName}.WithFunction()
self.assertEqual(withFunction.sum([1, 2, 3]), 6)
)",
                                    fmt::arg("moduleName", moduleName));

  auto errorCode = stage.runEmbindTest(cppCode, pythonTestCode, moduleName);
  REQUIRE(errorCode == 0);
}
