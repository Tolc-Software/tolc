#include "Embind/checkType.hpp"
#include "Embind/Builders/typeToStringBuilder.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "TestUtil/string.hpp"

#include <IR/ir.hpp>
#include <TestUtil/parse.hpp>
#include <TestUtil/types.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Construct register_vector", "[checkType]") {
  auto c = TestUtil::getVector();
  Embind::Proxy::TypeInfo typeInfo;
  Embind::checkType(c, typeInfo);
  REQUIRE(typeInfo.m_registerCommands.size() == 1);
  for (auto const& command : typeInfo.m_registerCommands) {
    CAPTURE(command);
    REQUIRE(command == "em::register_vector<int>(\"vector_int\")");
  }
}

TEST_CASE("Construct register_map", "[checkType]") {
  auto c = TestUtil::getMap();
  Embind::Proxy::TypeInfo typeInfo;
  Embind::checkType(c, typeInfo);
  REQUIRE(typeInfo.m_registerCommands.size() == 1);
  for (auto const& command : typeInfo.m_registerCommands) {
    CAPTURE(command);
    REQUIRE(command == "em::register_map<int, int>(\"map_int_int\")");
  }
}

TEST_CASE("Construct value_array", "[checkType]") {
  auto c = TestUtil::getArray();
  Embind::Proxy::TypeInfo typeInfo;
  Embind::checkType(c, typeInfo);
  REQUIRE(typeInfo.m_registerCommands.size() == 1);
  for (auto const& command : typeInfo.m_registerCommands) {
    CAPTURE(command);
    REQUIRE(TestUtil::contains(
        command, "em::value_array<std::array<int, 2>>(\"array_int_2\")"));
    REQUIRE(TestUtil::contains(command, ".element(emscripten::index<0>())"));
    REQUIRE(TestUtil::contains(command, ".element(emscripten::index<1>())"));
  }
}

TEST_CASE("Construct value_array for pair", "[checkType]") {
  auto c = TestUtil::getPair();
  Embind::Proxy::TypeInfo typeInfo;
  Embind::checkType(c, typeInfo);
  REQUIRE(typeInfo.m_registerCommands.size() == 1);
  for (auto const& command : typeInfo.m_registerCommands) {
    CAPTURE(command);
    REQUIRE(TestUtil::contains(
        command,
        "em::value_array<std::pair<int, std::string>>(\"pair_int_string\")"));
    REQUIRE(TestUtil::contains(
        command, R"(.element(&std::pair<int, std::string>::first))"));
    TestUtil::contains(command,
                       R"(.element(&std::pair<int, std::string>::second))");
  }
}

TEST_CASE("Construct value_array for tuple", "[checkType]") {
  auto c = TestUtil::getTuple();
  Embind::Proxy::TypeInfo typeInfo;
  Embind::checkType(c, typeInfo);
  REQUIRE(typeInfo.m_registerCommands.size() == 1);
  for (auto const& command : typeInfo.m_registerCommands) {
    CAPTURE(command);
    REQUIRE(TestUtil::contains(
        command,
        "em::value_array<std::tuple<int, std::string>>(\"tuple_int_string\")"));
    REQUIRE(TestUtil::contains(
        command,
        R"(.element(&Tolc_::tuple_int_string_get_0, &Tolc_::tuple_int_string_set_0))"));

    REQUIRE(TestUtil::contains(
        command,
        R"(.element(&Tolc_::tuple_int_string_get_1, &Tolc_::tuple_int_string_set_1))"));
  }
}
