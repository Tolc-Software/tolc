#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE(
    "Simple data class with one container of strings - should have an implicitly created default constructor if no other constructor is available",
    "[dataStructs]") {
  for (bool hasUserDefinedConstructor : {true, false}) {
    std::string constructor =
        hasUserDefinedConstructor ? "MyClass(std::string);" : "";
    for (std::string container : {"vector", "list", "set", "unordered_set"}) {
      for (std::string accessor : {"public", "private", "protected"}) {
        auto code = fmt::format(R"(
#include <string>
#include <{container}>

struct MyClass {{
	{constructor}
{accessor}:
	std::{container}<std::string> s;
}};
)",
                                fmt::arg("container", container),
                                fmt::arg("constructor", constructor),
                                fmt::arg("accessor", accessor));
        CAPTURE(code);
        auto globalNS = TestUtil::parseString(code);
        REQUIRE(globalNS.m_structs.size() == 1);
        auto& myStruct = globalNS.m_structs.back();
        REQUIRE(myStruct.m_hasImplicitDefaultConstructor ==
                !hasUserDefinedConstructor);
      }
    }
  }
}

TEST_CASE(
    "Class with constructor should not have implicitly created default constructor",
    "[dataStructs]") {
  auto code = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::string s) : m_s(s) {}

	std::string* getS() { return &m_s; }

private:
	std::string m_s;
};

class Owner {
public:
	explicit Owner(MyClass m) : m_myClass(m) {};

	MyClass getMyClass() const { return m_myClass; }

private:
	MyClass m_myClass;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_structs.size() == 2);
  for (auto const& s : globalNS.m_structs) {
    REQUIRE(s.m_hasImplicitDefaultConstructor == false);
  }
}
