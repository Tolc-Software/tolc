#include "Pybind/Builders/classBuilder.hpp"
#include "Pybind/Proxy/class.hpp"
#include "TestUtil/string.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

namespace {
IR::Struct getStruct(std::string const& name) {
	IR::Struct s;
	s.m_name = name;
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;
	return s;
}
struct Var {
	std::string name;
	std::string type;
	bool isConst;
};
}    // namespace

TEST_CASE("Class with static member variables", "[classBuilder]") {
	std::vector<Var> variables = {Var({"v0", "int", true}),
	                              Var({"myVar", "double", false})};

	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	for (auto const& var : variables) {
		IR::Variable v;
		v.m_name = var.name;
		IR::Type t;
		t.m_representation = var.type;
		t.m_isConst = var.isConst;
		v.m_isStatic = true;
		v.m_type = t;
		s.m_public.m_memberVariables.push_back(v);
	}

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	for (auto const& var : variables) {
		auto accessor = var.isConst ? "readonly" : "readwrite";
		auto expectedContains = fmt::format(
		    R"(def_{accessor}_static("{variableName}", &{className}::{variableName})",
		    fmt::arg("accessor", accessor),
		    fmt::arg("variableName", var.name),
		    fmt::arg("className", s.m_name));
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("Class with static function", "[classBuilder]") {
	std::string moduleName = "MyModule";
	auto s = getStruct("MyStruct");

	IR::Function f;

	f.m_name = "f";
	f.m_representation = f.m_name;
	f.m_isStatic = true;

	IR::Type returnType;
	returnType.m_representation = "void";
	f.m_returnType = returnType;

	s.m_public.m_functions.push_back(f);

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto expectedContains =
	    fmt::format("\t.def_static(\"{function}\", &{function}",
	                fmt::arg("function", f.m_name));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Templated class", "[classBuilder]") {
	std::string moduleName = "MyModule";
	std::string removedTemplatePars = "SomeClass_int";
	auto s = getStruct("SomeClass<int>");
	IR::Type t;
	t.m_isConst = false;
	t.m_isReference = false;
	t.m_numPointers = 0;
	t.m_representation = "int";
	auto v = IR::Type::Value();
	v.m_base = IR::BaseType::Int;
	t.m_type = v;
	s.m_templateArguments.push_back(t);

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	auto expectedContains = fmt::format(
	    R"(py::class_<{fullyQualifiedClassName}>({moduleName}, "{className}")",
	    fmt::arg("fullyQualifiedClassName", s.m_representation),
	    fmt::arg("moduleName", moduleName),
	    fmt::arg("className", removedTemplatePars));
	CAPTURE(pybind);
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Class within namespace", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = fmt::format("MyNamespace::{className}",
	                                 fmt::arg("className", s.m_name));
	s.m_hasImplicitDefaultConstructor = true;

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto expectedContains = fmt::format(
	    R"(py::class_<{fullyQualifiedClassName}>({moduleName}, "{className}")",
	    fmt::arg("fullyQualifiedClassName", s.m_representation),
	    fmt::arg("moduleName", moduleName),
	    fmt::arg("className", s.m_name));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Empty class gets default constructor", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto expectedContains = "def(py::init<>(), \"\")";
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Class with a constructor", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	IR::Function f;
	f.m_name = s.m_name;
	f.m_representation = f.m_name;
	f.m_isStatic = false;
	IR::Argument v;
	v.m_name = 's';
	IR::Type t;
	t.m_representation = "const std::string&";
	v.m_type = t;
	f.m_arguments.push_back(v);
	s.m_public.m_constructors.push_back(f);

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	auto expectedContains =
	    fmt::format(R"(.def(py::init<{type}>(), "", py::arg("s"))",
	                fmt::arg("type", t.m_representation));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}

TEST_CASE("Class with functions", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	std::vector<std::pair<std::string, std::string>> functions = {
	    std::make_pair("f", "int"),
	    std::make_pair("another_func", "std::string"),
	    std::make_pair("fun", "double")};
	for (auto const& [function, type] : functions) {
		IR::Function f;

		f.m_name = function;
		f.m_representation = f.m_name;
		f.m_isStatic = false;

		IR::Argument v;
		v.m_name = "myVar";

		IR::Type arg;
		arg.m_representation = type;
		v.m_type = arg;
		f.m_arguments.push_back(v);

		IR::Type returnType;
		returnType.m_representation = "void";
		f.m_returnType = returnType;

		s.m_public.m_functions.push_back(f);
	}

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	for (auto const& [function, argument] : functions) {
		auto expectedContains =
		    fmt::format("\t.def(\"{function}\", &{function}",
		                fmt::arg("function", function));
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("Class with member variables", "[classBuilder]") {
	std::vector<Var> variables = {Var({"v0", "int", true}),
	                              Var({"s", "const std::string&", false}),
	                              Var({"myVar", "double", false})};

	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_representation = s.m_name;
	s.m_hasImplicitDefaultConstructor = true;

	for (auto const& var : variables) {
		IR::Variable v;
		v.m_name = var.name;
		IR::Type t;
		t.m_representation = var.type;
		t.m_isConst = var.isConst;
		v.m_isStatic = false;
		v.m_type = t;
		s.m_public.m_memberVariables.push_back(v);
	}

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	CAPTURE(pybind);

	for (auto const& var : variables) {
		auto accessor = var.isConst ? "readonly" : "readwrite";
		auto expectedContains = fmt::format(
		    R"(def_{accessor}("{variableName}", &{className}::{variableName})",
		    fmt::arg("accessor", accessor),
		    fmt::arg("variableName", var.name),
		    fmt::arg("className", s.m_name));
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));
	}
}

TEST_CASE("Class with vector in constructor gives the correct include",
          "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "MyStruct";
	s.m_hasImplicitDefaultConstructor = true;
	IR::Function constructor;
	constructor.m_name = s.m_name;
	constructor.m_isStatic = false;
	IR::Type arg;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	arg.m_type = c;
	constructor.m_arguments.push_back({"myVar", arg});
	s.m_public.m_functions.push_back(constructor);

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "#include <pybind11/stl.h>");
	}
}

TEST_CASE("Class with vector in member function gives the correct include",
          "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "SomeClass";
	s.m_hasImplicitDefaultConstructor = true;
	IR::Function constructor;
	constructor.m_name = s.m_name;
	constructor.m_isStatic = false;
	IR::Type arg;
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	arg.m_type = c;
	constructor.m_arguments.push_back({"myVar", arg});
	s.m_public.m_functions.push_back(constructor);

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	REQUIRE(typeInfo.m_includes.size() == 1);
	for (auto const& include : typeInfo.m_includes) {
		REQUIRE(include == "#include <pybind11/stl.h>");
	}
}

TEST_CASE("Class with enum", "[classBuilder]") {
	std::string moduleName = "MyModule";
	IR::Struct s;
	s.m_name = "SomeClass";
	s.m_hasImplicitDefaultConstructor = true;
	IR::Enum e;
	e.m_isScoped = true;
	e.m_name = "MyEnum";
	e.m_representation = moduleName + "::" + e.m_name;
	e.m_values.push_back("Hi");
	s.m_public.m_enums.push_back(e);

	Pybind::Proxy::TypeInfo typeInfo;
	auto myStruct = Pybind::Builders::buildClass(s, typeInfo).value();
	auto pybind = myStruct.getPybind(moduleName);
	auto expectedContains = fmt::format(
	    R"(py::enum_<{representation}>({structureName}, "{enumName}")",
	    fmt::arg("representation", e.m_representation),
	    fmt::arg("enumName", e.m_name),
	    fmt::arg("structureName", s.m_name));
	CAPTURE(pybind);
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
}
