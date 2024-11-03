#include "Pybind/Proxy/module.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Modules defines their functions", "[module]") {
	std::string moduleName = "myModule";
	Pybind::Proxy::Module m(moduleName);

	std::vector<std::string> functions = {"f", "calculate", "foo"};
	for (auto const& function : functions) {
		m.addFunction(Pybind::Proxy::Function(function, function));
	}

	auto pybindCode = m.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& function : functions) {
		auto expectedContains =
		    fmt::format(R"({module}.def("{function}", &{function})",
		                fmt::arg("module", m.getVariableName()),
		                fmt::arg("function", function));
		CAPTURE(function);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}

TEST_CASE("Modules defines their submodules", "[module]") {
	std::string moduleName = "myTestModule";
	Pybind::Proxy::Module m(moduleName);

	std::vector<std::string> submodules = {"sub1", "mySub", "child"};
	for (auto const& submodule : submodules) {
		m.addSubmodule(submodule, moduleName + "__" + submodule, "");
	}

	auto pybindCode = m.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& submodule : submodules) {
		auto expectedContains = fmt::format(
		    R"(auto {moduleName}__{submodule} = {module}.def_submodule("{submodule}")",
		    fmt::arg("moduleName", moduleName),
		    fmt::arg("module", m.getVariableName()),
		    fmt::arg("submodule", submodule));
		CAPTURE(submodule);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}

TEST_CASE("Modules defines their classes", "[module]") {
	std::string moduleName = "myTestModule";
	Pybind::Proxy::Module m(moduleName);

	std::vector<std::string> classes = {"Cl", "MyClass", "OtherClass"};
	for (auto const& cls : classes) {
		Pybind::Proxy::Class c(cls, cls);
		m.addClass(c);
	}

	auto pybindCode = m.getPybind();
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& cls : classes) {
		auto expectedContains =
		    fmt::format("\tpy::class_<{cls}>({moduleName}, \"{cls}\"",
		                fmt::arg("cls", cls),
		                fmt::arg("moduleName", m.getVariableName()));
		CAPTURE(cls);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}

TEST_CASE("Modules gets a somewhat unique variable name", "[module]") {
	std::string moduleName = "myTestModule";
	std::string variableName = std::string("rootModule__") + moduleName;
	Pybind::Proxy::Module m(variableName);

	using TestUtil::contains;
	REQUIRE(variableName == "rootModule__myTestModule");
}

TEST_CASE("Module variable name", "[module]") {
	std::string moduleName = "myTestModule";
	std::string variableName = moduleName + "__ns";
	Pybind::Proxy::Module m(variableName);

	using TestUtil::contains;
	REQUIRE(m.getVariableName() == "myTestModule__ns");
}
