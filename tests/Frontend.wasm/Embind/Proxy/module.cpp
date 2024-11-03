#include "Embind/Proxy/module.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Modules defines their functions", "[module]") {
  std::string moduleName = "myModule";
  Embind::Proxy::Module m(moduleName, moduleName);

  std::vector<std::string> functions = {"f", "calculate", "foo"};
  for (auto const& function : functions) {
    m.addFunction(
        Embind::Proxy::Function(function, moduleName + "::" + function));
  }

  auto embindCode = m.getEmbind();
  CAPTURE(embindCode);

  using TestUtil::contains;
  for (auto const& function : functions) {
    auto expectedContains =
        fmt::format(R"(function("myModule_{function}", &myModule::{function})",
                    fmt::arg("function", function));
    CAPTURE(function);
    CAPTURE(expectedContains);
    REQUIRE(contains(embindCode, expectedContains));
  }
}

TEST_CASE("PreJS includes function", "[module]") {
  std::string moduleName = "myModule";
  Embind::Proxy::Module m(moduleName, moduleName);

  std::vector<std::string> functions = {"f", "calculate", "foo"};
  for (auto const& function : functions) {
    m.addFunction(
        Embind::Proxy::Function(function, moduleName + "::" + function));
  }

  auto preJS = m.getPreJS();
  CAPTURE(preJS);
  using TestUtil::contains;
  for (auto const& function : functions) {
    auto expectedContains =
        fmt::format(R"({function}: Module['myModule_{function}'],)",
                    fmt::arg("function", function));
    CAPTURE(function);
    CAPTURE(expectedContains);
    REQUIRE(contains(preJS, expectedContains));
  }
}

// TEST_CASE("Module variable name", "[module]") {
// 	std::string moduleName = "myTestModule";
// 	std::string variableName = moduleName + "__ns";
// 	Embind::Proxy::Module m(variableName);

// 	using TestUtil::contains;
// 	REQUIRE(m.getPrefix() == "myTestModule__ns");
// }

// TEST_CASE("Modules defines their classes", "[module]") {
// 	std::string moduleName = "myTestModule";
// 	Embind::Proxy::Module m(moduleName);

// 	std::vector<std::string> classes = {"Cl", "MyClass", "OtherClass"};
// 	for (auto const& cls : classes) {
// 		Embind::Proxy::Class c(cls, cls);
// 		m.addClass(c);
// 	}

// 	auto embindCode = m.getEmbind();
// 	CAPTURE(embindCode);

// 	using TestUtil::contains;
// 	for (auto const& cls : classes) {
// 		auto expectedContains =
// 		    fmt::format("\tpy::class_<{cls}>({moduleName}, \"{cls}\");\n",
// 		                fmt::arg("cls", cls),
// 		                fmt::arg("moduleName", m.getPrefix()));
// 		CAPTURE(cls);
// 		CAPTURE(expectedContains);
// 		REQUIRE(contains(embindCode, expectedContains));
// 	}
// }
