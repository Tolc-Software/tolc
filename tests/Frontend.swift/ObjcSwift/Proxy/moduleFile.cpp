#include "ObjcSwift/Proxy/moduleFile.hpp"
#include "ObjcSwift/Proxy/function.hpp"
#include "ObjcSwift/Proxy/module.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
  std::string moduleName = "myModule";
  ObjcSwift::Proxy::Module m(moduleName);
  ObjcSwift::Proxy::Function f("f", "f");
  m.addFunction(f);
  ObjcSwift::Proxy::ModuleFile mf(m, moduleName);

  auto pybindCode = mf.getObjcSwift();
  CAPTURE(pybindCode);

  // The module
  REQUIRE(TestUtil::contains(
      pybindCode,
      fmt::format("PYBIND11_MODULE({}, {})", moduleName, m.getVariableName())));
  // The module contains the function
  REQUIRE(
      TestUtil::contains(pybindCode, m.getVariableName() + R"(.def("f", )"));
}
