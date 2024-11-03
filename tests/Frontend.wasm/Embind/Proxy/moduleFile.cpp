#include "Embind/Proxy/moduleFile.hpp"
#include "Embind/Proxy/function.hpp"
#include "Embind/Proxy/module.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("ModuleFile includes embind and default start of bindings",
          "[moduleFile]") {
  std::string moduleName = "myModule";
  Embind::Proxy::Module m(moduleName, moduleName);
  Embind::Proxy::ModuleFile mf(m, moduleName);

  auto embindCode = mf.getEmbind();
  CAPTURE(embindCode);

  // The include
  REQUIRE(TestUtil::contains(embindCode, "#include <emscripten/bind.h>"));

  // The default start of export
  REQUIRE(TestUtil::contains(
      embindCode, fmt::format("EMSCRIPTEN_BINDINGS({})", moduleName)));
}

TEST_CASE("ModuleFile includes the default preJS", "[moduleFile]") {
  std::string moduleName = "myModule";
  Embind::Proxy::Module m(moduleName, moduleName);
  Embind::Proxy::ModuleFile mf(m, moduleName);

  auto preJSCode = mf.getPreJS();
  CAPTURE(preJSCode);

  using TestUtil::contains;
  // The Module declaration
  REQUIRE(contains(preJSCode, "var Module = {"));

  // The postRun start
  REQUIRE(contains(preJSCode, "postRun: [() => {"));
}

// TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
// 	std::string moduleName = "myModule";
// 	Embind::Proxy::Module m(moduleName);
// 	Embind::Proxy::Function f("f", "f");
// 	m.addFunction(f);
// 	Embind::Proxy::ModuleFile mf(m, moduleName);

// 	auto embindCode = mf.getEmbind();
// 	CAPTURE(embindCode);

// 	// The module
// 	REQUIRE(TestUtil::contains(
// 	    embindCode,
// 	    fmt::format("PYBIND11_MODULE({}, {})", moduleName, m.getPrefix())));
// 	// The module contains the function
// 	REQUIRE(TestUtil::contains(embindCode, m.getPrefix() + R"(.def("f", )"));
// }
