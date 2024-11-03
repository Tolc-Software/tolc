#include "Embind/Proxy/class.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <optional>
#include <string>
#include <vector>

TEST_CASE("Empty class", "[class]") {
  std::string className = "myClass";
  Embind::Proxy::Class c(className, className);
  auto embindCode = c.getEmbind();
  CAPTURE(embindCode);

  using TestUtil::contains;
  auto expectedContains = fmt::format(R"(class_<{className}>("{className}"))",
                                      fmt::arg("className", className));
  CAPTURE(expectedContains);
  REQUIRE(contains(embindCode, expectedContains));
}

TEST_CASE("Class with functions", "[class]") {
  std::string className = "myOtherClass";
  Embind::Proxy::Class c(className, className);

  std::vector<std::string> functions = {"f", "calculate", "foo"};
  for (auto const& function : functions) {
    auto f = Embind::Proxy::Function(function, function);
    f.setAsClassFunction();
    c.addFunction(f);
  }

  auto embindCode = c.getEmbind();
  CAPTURE(embindCode);
  std::string classDecl =
      fmt::format("class_<{}>(\"{}\")", className, className);
  CAPTURE(classDecl);

  using TestUtil::contains;

  REQUIRE(contains(embindCode, classDecl));

  for (auto const& function : functions) {
    auto expectedContains =
        fmt::format("\t\t.function(\"{function}\", &{function}",
                    fmt::arg("function", function));
    CAPTURE(function);
    CAPTURE(expectedContains);
    REQUIRE(contains(embindCode, expectedContains));
  }
}

TEST_CASE("Class with constructor", "[class]") {
  std::string className = "myFreshClass";
  Embind::Proxy::Class c(className, className);

  std::vector<std::string> arguments = {"const std::string&", "int"};
  auto constructor = Embind::Proxy::Function(className, className);
  for (auto const& argument : arguments) {
    constructor.addArgument(argument);
  }
  constructor.setAsConstructor();
  c.addConstructor(constructor);

  auto embindCode = c.getEmbind();
  CAPTURE(embindCode);

  using TestUtil::contains;
  auto expectedContains =
      fmt::format("\t\t.constructor<{}>()", fmt::join(arguments, ", "));
  CAPTURE(expectedContains);
  REQUIRE(contains(embindCode, expectedContains));
}

TEST_CASE("Class with member variables", "[class]") {
  std::string className = "SuperbClass";
  Embind::Proxy::Class c(className, className);

  std::vector<std::string> constVariables = {"myInt", "var", "yes"};
  for (auto const& variable : constVariables) {
    c.addMemberVariable(variable, "MyGetter_" + variable, std::nullopt, false);
  }

  std::vector<std::string> nonConstVariables = {"myOtherInt", "var2", "no"};
  for (auto const& variable : nonConstVariables) {
    c.addMemberVariable(
        variable, "MyGetter_" + variable, "MySetter_" + variable, false);
  }

  auto embindCode = c.getEmbind();
  CAPTURE(embindCode);

  using TestUtil::contains;
  for (auto const& variable : constVariables) {
    auto expectedProperty = fmt::format("\t\t.property(\"{variable}\", ",
                                        fmt::arg("variable", variable));

    CAPTURE(expectedProperty);
    REQUIRE(contains(embindCode, expectedProperty));

    auto expectedGetter = fmt::format(R"(&MyGetter_{variableName})",
                                      fmt::arg("variableName", variable));
    CAPTURE(expectedGetter);
    REQUIRE(TestUtil::contains(embindCode, expectedGetter));
  }

  for (auto const& variable : nonConstVariables) {
    auto expectedProperty = fmt::format("\t\t.property(\"{variable}\", ",
                                        fmt::arg("variable", variable));

    auto expectedGetter = fmt::format(R"(&MyGetter_{variableName})",
                                      fmt::arg("variableName", variable));

    auto expectedSetter = fmt::format(R"(&MySetter_{variableName})",
                                      fmt::arg("variableName", variable));

    CAPTURE(expectedProperty);
    CAPTURE(expectedGetter);
    CAPTURE(expectedSetter);
    REQUIRE(contains(embindCode, expectedProperty));
    REQUIRE(contains(embindCode, expectedGetter));
    REQUIRE(contains(embindCode, expectedSetter));
  }
}

TEST_CASE("Static variable", "[class]") {
  std::string className = "myClass";
  Embind::Proxy::Class c(className, className);
  c.addMemberVariable("v", "v", std::nullopt, true);
  auto embindCode = c.getEmbind();
  CAPTURE(embindCode);

  using TestUtil::contains;
  auto expectedProperty = fmt::format(".class_property(\"v\", &v)");
  CAPTURE(expectedProperty);
  REQUIRE(contains(embindCode, expectedProperty));
}

TEST_CASE("PreJS", "[class]") {
  std::string ns = "Stuff";
  std::string className = "myClass";
  Embind::Proxy::Class c(className, ns + "::" + className);
  std::vector<std::string> namesToDelete;
  auto preJS = c.getPreJS(ns + "_", namesToDelete);
  CAPTURE(preJS);

  using TestUtil::contains;
  auto expectedProperty =
      fmt::format("{}: Module['{}'],", className, ns + "_" + className);
  CAPTURE(expectedProperty);
  REQUIRE(contains(preJS, expectedProperty));
}

TEST_CASE("GlobalPreJS", "[class]") {
  std::string ns = "Stuff";
  std::string className = "myClass";
  Embind::Proxy::Class c(className, ns + "::" + className);
  std::vector<std::string> namesToDelete;
  auto noEnumsPrejs = c.getGlobalPreJS(ns + "_", namesToDelete);
  REQUIRE(noEnumsPrejs.empty());

  std::string enumName = "MyEnum";
  std::string fullyQualifiedName = className + "::" + enumName;
  Embind::Proxy::Enum e(enumName, fullyQualifiedName);
  e.addValue("Hi");
  c.addEnum(e);
  auto preJS = c.getGlobalPreJS(ns + "_", namesToDelete);

  CAPTURE(preJS);

  using TestUtil::contains;
  auto expectedProperty = fmt::format(
      "Module['{globalClassName}']['{enumName}'] = Module['{globalEnumName}'];",
      fmt::arg("globalClassName", ns + "_" + className),
      fmt::arg("enumName", enumName),
      fmt::arg("globalEnumName", ns + "_" + className + "_" + enumName));
  CAPTURE(expectedProperty);
  REQUIRE(contains(preJS, expectedProperty));
}

TEST_CASE("Class with enum", "[class]") {
  std::string className = "myFreshClass";
  Embind::Proxy::Class c(className, className);

  std::string enumName = "MyEnum";
  std::string fullyQualifiedName = className + "::" + enumName;
  Embind::Proxy::Enum e(enumName, fullyQualifiedName);
  std::string valueName = "Hi";
  e.addValue(valueName);
  c.addEnum(e);

  using TestUtil::contains;
  for (std::string const& expectedContains :
       {fmt::format(R"(em::class_<{className}>("{className}");)",
                    fmt::arg("className", className)),
        fmt::format(R"(em::enum_<{fullyQualifiedName}>("{globalEnumName}"))",
                    fmt::arg("fullyQualifiedName", fullyQualifiedName),
                    fmt::arg("globalEnumName", className + "_" + enumName)),
        fmt::format(R"(.value("{valueName}", {fullyQualifiedValueName});)",
                    fmt::arg("fullyQualifiedValueName",
                             fullyQualifiedName + "::" + valueName),
                    fmt::arg("valueName", valueName))}) {
    auto embind = c.getEmbind();
    CAPTURE(embind);
    CAPTURE(expectedContains);
    REQUIRE(TestUtil::contains(embind, expectedContains));
  }
}
