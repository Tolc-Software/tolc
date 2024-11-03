#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Overloaded functions", "[overloadedFunctions]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

// Overloaded free functions
std::string sayHello() {
	return "Hello!";
}

std::string sayHello(std::string to) {
	return std::string("Hello ") + to;
}

std::string sayHello(size_t times) {
	std::string greeting = "";
	for (size_t i = 0; i < times; ++i) {
		greeting += "Hello!";
	}
	return greeting;
}

class Overload {
public:
	// Overloaded constructor
	Overload() : m_s() {};
	Overload(std::string s) : m_s(s) {};

	// Overloaded class functions
	std::string getStuff() { return "Stuff"; }
	std::string getStuff(std::string customStuff) { return customStuff; }

private:
	std::string m_s;
};
)";

  auto objcTestCode = R"(
// Overloaded functions work the same as in C++
// Free function overload
assert([[m sayHello] isEqualToString:@"Hello!"]);
assert([[m sayHelloString:@"Tolc"] isEqualToString:@"Hello Tolc"]);
assert([[m sayHelloUnsignedLongInt:2] isEqualToString:@"Hello!Hello!"]);

// Class constructor overload
mOverload* overload = [[mOverload alloc] init];
mOverload* overloadWithString = [[mOverload alloc] initWithString:@"Overloaded!"];

// Class function overload
assert([[overload getStuff] isEqualToString:@"Stuff"]);
assert([[overload getStuffString:@"Other"] isEqualToString:@"Other"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Overloaded Functions");
}
