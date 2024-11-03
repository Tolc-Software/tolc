#include "Helpers/Type/matchers.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("User defined strings are parsed correctly", "[matchers]") {
  for (auto const& className : {"MyClass", "struct", "structeD"}) {
    for (std::string declaration : {"struct ", "class "}) {
      std::string userDefined = declaration + className;
      CAPTURE(userDefined);
      auto ud = Helpers::Type::getUserDefinedType(userDefined);
      REQUIRE(ud.has_value());
      REQUIRE(ud.value().m_representation == className);
    }
  }
}

TEST_CASE("Non user defined types are rejected", "[matchers]") {
  for (auto const& type : {"int", "double", "whateverStruct"}) {
    CAPTURE(type);
    auto ud = Helpers::Type::getUserDefinedType(type);
    REQUIRE(!ud.has_value());
  }
}
