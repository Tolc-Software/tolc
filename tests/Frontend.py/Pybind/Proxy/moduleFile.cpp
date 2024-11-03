#include "Pybind/Proxy/moduleFile.hpp"
#include "Pybind/Proxy/function.hpp"
#include "Pybind/Proxy/module.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("ModuleFile can take a module", "[moduleFile]") {
	std::string moduleName = "myModule";
	Pybind::Proxy::Module m(moduleName);
	Pybind::Proxy::Function f("f", "f");
	m.addFunction(f);
	Pybind::Proxy::ModuleFile mf(m, moduleName);

	auto pybindCode = mf.getPybind();
	CAPTURE(pybindCode);

	// The module
	REQUIRE(TestUtil::contains(pybindCode,
	                           fmt::format("PYBIND11_MODULE({}, {})",
	                                       moduleName,
	                                       m.getVariableName())));
	// The module contains the function
	REQUIRE(
	    TestUtil::contains(pybindCode, m.getVariableName() + R"(.def("f", )"));
}
