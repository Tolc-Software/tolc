#include "Embind/Proxy/enum.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Enums", "[enum]") {
  std::string moduleName = "MyModule";
  std::string name = "MyEnum";
  std::string fullyQualifiedName = moduleName + "::" + name;

  Embind::Proxy::Enum e(name, fullyQualifiedName);
  std::string value = "MyValue";
  e.addValue(value);

  auto embind = e.getEmbind();

  auto expectedContains =
      fmt::format(R"(em::enum_<{fullyQualifiedName}>("{name}"))",
                  fmt::arg("fullyQualifiedName", fullyQualifiedName),
                  fmt::arg("name", name));

  auto valueExists =
      fmt::format(R"(.value("{value}", {fullyQualifiedName}::{value}))",
                  fmt::arg("value", value),
                  fmt::arg("fullyQualifiedName", fullyQualifiedName));

  CAPTURE(embind);
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(embind, expectedContains));

  CAPTURE(valueExists);
  REQUIRE(TestUtil::contains(embind, valueExists));
}

TEST_CASE("PreJS enum", "[enum]") {
  std::string moduleName = "MyModule";
  std::string name = "MyEnum";
  std::string fullyQualifiedName = moduleName + "::" + name;

  Embind::Proxy::Enum e(name, fullyQualifiedName);
  std::string value = "MyValue";
  e.addValue(value);

  std::vector<std::string> previousNames;
  auto prejs = e.getPreJS("MyModule_", previousNames);

  auto expectedContains = "MyEnum: Module['MyModule_MyEnum']";

  CAPTURE(prejs);
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(prejs, expectedContains));
  REQUIRE(previousNames.size() == 1);
  REQUIRE(previousNames.back() == "MyModule_MyEnum");
}
