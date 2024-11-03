#include "Parser/Parse.hpp"
#include "TestUtil/files.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Incorrect C++ does not emit a namespace", "[Parse]") {
  auto code = R"(
class MyClass {
	// Using string without including the header
	std::string s;
};
)";
  CAPTURE(code);
  auto ns = Parser::parseString(code);
  REQUIRE(!ns.has_value());

  auto file = TestUtil::writeToFile(code);
  auto nsFromFile = Parser::parseFile(file);
  REQUIRE(!nsFromFile.has_value());
}

TEST_CASE("C++ code within a file does not depend on the file extension",
          "[Parse]") {
  for (auto extension :
       {".h", ".c", ".txt", "", ".cc", ".cpp", ".hpp", ".cxx", ".py"}) {
    auto code = R"(
class MyClass {
	int i;
};
)";
    CAPTURE(extension);
    auto ns = Parser::parseString(code);
    REQUIRE(ns.has_value());

    auto file = TestUtil::writeToFile(code, extension);
    auto nsFromFile = Parser::parseFile(file);
    REQUIRE(nsFromFile.has_value());
  }
}

TEST_CASE("System includes works with default config", "[Parse]") {
  auto code = R"(
#include <string>

class MyClass {
	// Using string without including the header
	std::string s;
};
)";

  // System includes "just works"
  REQUIRE(Parser::parseString(code).has_value());
}
