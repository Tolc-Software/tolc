#include "Pybind/Proxy/class.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Empty class", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myClass";
	Pybind::Proxy::Class c(className, className);
	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	auto expectedContains =
	    fmt::format(R"(py::class_<{className}>({moduleName}, "{className}")",
	                fmt::arg("className", className),
	                fmt::arg("moduleName", moduleName));
	CAPTURE(expectedContains);
	REQUIRE(contains(pybindCode, expectedContains));
}

TEST_CASE("Class with functions", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myOtherClass";
	Pybind::Proxy::Class c(className, className);

	std::vector<std::string> functions = {"f", "calculate", "foo"};
	for (auto const& function : functions) {
		c.addFunction(Pybind::Proxy::Function(function, function));
	}

	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& function : functions) {
		auto expectedContains =
		    fmt::format("\t.def(\"{function}\", &{function}",
		                fmt::arg("function", function));
		CAPTURE(function);
		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}

TEST_CASE("Class with enum", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myFreshClass";
	Pybind::Proxy::Class c(className, className);

	std::string enumName = "MyEnum";
	std::string fullyQualifiedName = moduleName + "::" + enumName;
	Pybind::Proxy::Enum e(enumName, fullyQualifiedName);
	e.setScoped(false);
	e.addValue("Hi");
	c.addEnum(e);

	using TestUtil::contains;
	auto expectedContains = fmt::format(
	    R"(py::enum_<{fullyQualifiedName}>({className}, "{enumName}")",
	    fmt::arg("fullyQualifiedName", fullyQualifiedName),
	    fmt::arg("enumName", enumName),
	    fmt::arg("className", className));
	auto pybind = c.getPybind(moduleName);
	CAPTURE(pybind);
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
	// Require exporting since it is scoped
	std::string exportValues = "\t.export_values()";
	CAPTURE(exportValues);
	REQUIRE(TestUtil::contains(pybind, exportValues));
}

TEST_CASE("Class with constructor", "[class]") {
	std::string moduleName = "myModule";
	std::string className = "myFreshClass";
	Pybind::Proxy::Class c(className, className);

	std::vector<std::string> arguments = {"const std::string&", "int"};
	auto constructor = Pybind::Proxy::Function(className, className);
	for (auto const& argument : arguments) {
		constructor.addArgument(argument);
	}
	constructor.setAsConstructor();
	c.addConstructor(constructor);

	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	auto expectedContains =
	    fmt::format("\t.def(py::init<{}>()", fmt::join(arguments, ", "));
	CAPTURE(expectedContains);
	REQUIRE(contains(pybindCode, expectedContains));
}

TEST_CASE("Class with member variables", "[class]") {
	std::string moduleName = "NewlyMadeModule";
	std::string className = "SuperbClass";
	Pybind::Proxy::Class c(className, className);

	std::vector<std::string> constVariables = {"myInt", "var", "yes"};
	for (auto const& variable : constVariables) {
		c.addMemberVariable(variable, "", true, false);
	}

	std::vector<std::string> nonConstVariables = {"myOtherInt", "var2", "no"};
	for (auto const& variable : nonConstVariables) {
		c.addMemberVariable(variable, "", false, false);
	}

	auto pybindCode = c.getPybind(moduleName);
	CAPTURE(pybindCode);

	using TestUtil::contains;
	for (auto const& variable : constVariables) {
		auto expectedContains = fmt::format(
		    "\t.def_readonly(\"{variable}\", &{className}::{variable}",
		    fmt::arg("variable", variable),
		    fmt::arg("className", className));

		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}

	for (auto const& variable : nonConstVariables) {
		auto expectedContains = fmt::format(
		    "\t.def_readwrite(\"{variable}\", &{className}::{variable}",
		    fmt::arg("variable", variable),
		    fmt::arg("className", className));

		CAPTURE(expectedContains);
		REQUIRE(contains(pybindCode, expectedContains));
	}
}
