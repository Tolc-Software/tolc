#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("Function returning a string", "[strings]") {
  auto code = R"(
#include <string>

std::string f();
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  auto& returnType = f.m_returnType;
  REQUIRE(returnType.m_representation == "std::string");

  auto& str = returnType.m_type;
  auto stringType = std::get_if<IR::Type::Value>(&str);
  REQUIRE(stringType != nullptr);

  REQUIRE(stringType->m_base == IR::BaseType::String);
}

TEST_CASE("Function returning a string_view", "[strings]") {
  auto code = R"(
#include <string_view>

std::string_view f();
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  auto& returnType = f.m_returnType;
  REQUIRE(returnType.m_representation == "std::string_view");

  auto& str = returnType.m_type;
  auto stringType = std::get_if<IR::Type::Value>(&str);
  REQUIRE(stringType != nullptr);

  REQUIRE(stringType->m_base == IR::BaseType::StringView);
}

TEST_CASE("String nested in a container", "[strings]") {
  auto code = R"(
#include <string>
#include <vector>

std::vector<std::string> f();
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  auto& returnType = f.m_returnType;
  REQUIRE(returnType.m_representation == "std::vector<std::string>");

  auto& vec = returnType.m_type;
  auto vecType = std::get_if<IR::Type::Container>(&vec);
  REQUIRE(vecType != nullptr);

  REQUIRE(vecType->m_container == IR::ContainerType::Vector);
  // A vector also has an allocator
  REQUIRE(vecType->m_containedTypes.size() == 2);
  // But the first one should be the string
  auto str = vecType->m_containedTypes.front();

  // As of writing this only the top level type is what the user wrote
  // (the nested levels removes aliases and using clauses so that we can indentify the type)
  // The result here is typically "std::basic_string<char, std::char_traits<char>, std::allocator<char>>"
  // but is dependent on the standard library used
  // REQUIRE(str.m_representation == "std::string");
  auto stringType = std::get_if<IR::Type::Value>(&str.m_type);
  REQUIRE(stringType != nullptr);

  REQUIRE(stringType->m_base == IR::BaseType::String);
}
