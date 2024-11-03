#include "Embind/Proxy/function.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Global function and class functions are the same", "[function]") {
  Embind::Proxy::Function f("f", "MyNamespace::f");

  auto globalFunction = f.getEmbind();
  f.setAsClassFunction();
  auto classFunction = f.getEmbind();
  CAPTURE(globalFunction);
  CAPTURE(classFunction);

  for (auto const& func : {globalFunction, classFunction}) {
    REQUIRE(TestUtil::contains(func, R"(function("f", &MyNamespace::f))"));
  }
}

TEST_CASE("Static class functions are defined with class_function",
          "[function]") {
  Embind::Proxy::Function f("f", "f");

  f.setAsClassFunction();
  f.setAsStatic();

  auto embindCode = f.getEmbind();
  CAPTURE(embindCode);

  REQUIRE(TestUtil::contains(embindCode, R"(class_function("f", &f))"));
}

TEST_CASE("Overloads have select_overload passed", "[function]") {
  Embind::Proxy::Function f("f", "NS::f");

  f.addArgument("int", "i");
  f.setAsOverloaded();

  auto embindCode = f.getEmbind();
  CAPTURE(embindCode);

  REQUIRE(TestUtil::contains(
      embindCode,
      R"(function("f_int", select_overload<(void(NS::*)(int))>(&NS::f)))"));
}

TEST_CASE("Void overloads is just called the function", "[function]") {
  Embind::Proxy::Function f("f", "NS::f");

  f.setAsOverloaded();

  auto embindCode = f.getEmbind();
  CAPTURE(embindCode);

  REQUIRE(TestUtil::contains(embindCode, R"(function("f",)"));
}

TEST_CASE("Function with arguments does not affect signature", "[function]") {
  Embind::Proxy::Function f("f", "f");
  std::vector<std::string> args = {"i", "j", "k"};
  for (auto const& arg : args) {
    f.addArgument("int", arg);
  }

  auto embindCode = f.getEmbind();
  CAPTURE(embindCode);

  // The function bind
  REQUIRE(TestUtil::contains(embindCode, R"(function("f", &f))"));
}

TEST_CASE(
    "PreJS sets the base name to the global name within the Module Object",
    "[function]") {
  Embind::Proxy::Function f("f", "MyNamespace::f");

  std::vector<std::string> previousNames;
  auto preJS = f.getPreJS("MyNamespace_", previousNames);
  CAPTURE(preJS);

  // The function bind
  REQUIRE(TestUtil::contains(preJS, R"(f: Module['MyNamespace_f'],)"));
  REQUIRE(previousNames.size() == 1);
  REQUIRE(previousNames.back() == "MyNamespace_f");
}
