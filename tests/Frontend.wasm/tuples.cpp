#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Using std::tuples", "[tuples]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>
#include <tuple>

class MyClass {
public:
	explicit MyClass(std::tuple<std::string, int> _tuple) : m_tuple(_tuple) {}

	std::tuple<std::string, int> getTuple() { return m_tuple; }

	std::tuple<std::string, int> m_tuple;
};

class WithFunction {
public:
	double sum(std::tuple<int, int, float, double> t) {
		return std::get<0>(t)
			   + std::get<1>(t)
			   + std::get<2>(t)
			   + std::get<3>(t);
	}
};

)";

  auto jsTestCode = R"(

// Tuple converts from javascript array
const myArray = ["Hello World", 42];
const myClass = new m.MyClass(myArray);
expect(myClass.getTuple()).toStrictEqual(myArray);

// The array still need to match the underlying std::tuple structure
try {
	// m_tuple is public
	myClass.m_tuple = [1, 2, 3];
} catch (err) {
	expect(err.toString()).toMatch(/TypeError: Incorrect number of tuple elements for tuple_string_int: expected=2, actual=3/i);
}

myClass.delete();

// Can handle different Number types
const withFunction = new m.WithFunction();
expect(withFunction.sum([1, 2, 3.3, 2.0])).toBeCloseTo(8.3, 5);

withFunction.delete();
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("std::tuple");
}
