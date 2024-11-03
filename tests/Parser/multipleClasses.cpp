#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Test case from frontend.py", "[classes]") {
  auto code = R"(
#include <string>

namespace MyLib {

	class Nested {
	public:
		double divideByTwo(double d) {
			return d / 2;
		}
	};
}

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}
	std::string getS() { return m_s; }
private:
	std::string m_s;
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};

class WithPrivateFunction {
	double multiply(double i, double j) {
		return i * j;
	}
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto& myLib = globalNS.m_namespaces.back();
  REQUIRE(myLib.m_structs.size() == 1);
  auto& nested = myLib.m_structs.back();
  REQUIRE(nested.m_name == "Nested");

  auto mwithConstructor =
      TestUtil::findWithName("WithConstructor", globalNS.m_structs);
  REQUIRE(mwithConstructor.has_value());
  auto withConstructor = mwithConstructor.value();
  REQUIRE(withConstructor.m_public.m_functions.size() == 1);
  REQUIRE(withConstructor.m_public.m_constructors.size() == 1);

  auto mwithFunction =
      TestUtil::findWithName("WithFunction", globalNS.m_structs);
  REQUIRE(mwithConstructor.has_value());
  auto withFunction = mwithFunction.value();
  REQUIRE(withFunction.m_public.m_functions.size() == 1);

  auto mwithPrivateFunction =
      TestUtil::findWithName("WithPrivateFunction", globalNS.m_structs);
  REQUIRE(mwithConstructor.has_value());
  auto withPrivateFunction = mwithPrivateFunction.value();
  REQUIRE(withPrivateFunction.m_private.m_functions.size() == 1);
}
