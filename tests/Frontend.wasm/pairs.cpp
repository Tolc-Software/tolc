#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Using std::pairs", "[pairs]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::pair<std::string, int> s) : m_s(s) {}

	std::pair<std::string, int> getS() { return m_s; }

private:
	std::pair<std::string, int> m_s;
};

class WithFunction {
public:
	int sum(std::pair<int, int> v) {
		return v.first + v.second;
	}
};

)";

  auto jsTestCode = R"(
// On the javascript side, std::pair<std::string, int> is a basic array
const myArray = ["hi", 4];
withMember = new m.MyClass(myArray);
expect(withMember.getS()).toStrictEqual(myArray);
withMember.delete();

const withFunction = new m.WithFunction()
expect(withFunction.sum([1, 2])).toBe(3)
withFunction.delete();
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::pair");
}
