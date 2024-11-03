#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <variant>

TEST_CASE("A simple user defined struct", "[userDefined]") {
  REQUIRE(1 == 1);
  auto globalNS = TestUtil::parseString(R"(
struct Simple {};

const Simple* f();
)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions[0];
  REQUIRE(f.m_name == "f");
  REQUIRE(f.m_returnType.m_representation == "const Simple *");
  auto returnType = std::get_if<IR::Type::UserDefined>(&f.m_returnType.m_type);
  REQUIRE(returnType);
  REQUIRE(returnType->m_representation == "Simple");
}

TEST_CASE("A user defined class within a namespace", "[userDefined]") {
  REQUIRE(1 == 1);
  auto globalNS = TestUtil::parseString(R"(
namespace Nested {

struct Simple {};
}

Nested::Simple& f();
)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions[0];
  REQUIRE(f.m_name == "f");
  REQUIRE(f.m_returnType.m_representation == "Nested::Simple &");
  auto returnType = std::get_if<IR::Type::UserDefined>(&f.m_returnType.m_type);
  REQUIRE(returnType);
  REQUIRE(returnType->m_representation == "Nested::Simple");
}

TEST_CASE("Two nested user defined classes", "[userDefined]") {
  REQUIRE(1 == 1);
  auto globalNS = TestUtil::parseString(R"(
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

Owner const& f(MyClass const& m);
)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions[0];
  REQUIRE(f.m_name == "f");
  REQUIRE(f.m_returnType.m_representation == "const Owner &");
  REQUIRE(f.m_arguments.size() == 1);
  auto& arg = f.m_arguments.back();
  REQUIRE(arg.m_type.m_isConst);
  REQUIRE(arg.m_type.m_isReference);
  REQUIRE(arg.m_type.m_representation == "const MyClass &");
}
