#include "Embind/Builders/classBuilder.hpp"
#include "Embind/Proxy/class.hpp"
#include "TestUtil/string.hpp"
#include "TestUtil/types.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <utility>
#include <vector>

namespace {
struct Var {
  std::string name;
  std::string type;
  bool isConst;
};
}    // namespace

TEST_CASE("Class with vector in member function gives the correct register",
          "[classBuilder]") {
  auto constructor = TestUtil::getFunction("SomeClass");
  IR::Argument arg;
  arg.m_name = "myVar";
  arg.m_type = TestUtil::getVector();
  constructor.m_arguments.push_back(arg);
  constructor.m_returnType = TestUtil::getMap();

  auto s = TestUtil::getStruct("SomeClass");
  s.m_public.m_functions.push_back(constructor);

  Embind::Proxy::TypeInfo typeInfo;
  auto c = Embind::Builders::buildClass(s, typeInfo);
  REQUIRE(c.has_value());
  REQUIRE(typeInfo.m_registerCommands.size() == 2);
  auto& regs = typeInfo.m_registerCommands;
  for (auto const& r : {R"(em::register_vector<int>("vector_int"))",
                        R"(em::register_map<int, int>("map_int_int"))"}) {
    CAPTURE(r);
    REQUIRE(regs.find(r) != regs.end());
  }
}

TEST_CASE("Class with a constructor", "[classBuilder]") {
  auto constructor = TestUtil::getFunction("SomeClass");
  IR::Argument arg;
  arg.m_name = "myVar";
  arg.m_type = TestUtil::getVector();
  constructor.m_arguments.emplace_back(arg);

  auto s = TestUtil::getStruct("SomeClass");
  s.m_public.m_constructors.push_back(constructor);

  Embind::Proxy::TypeInfo typeInfo;
  auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
  auto embind = myStruct.getEmbind();
  CAPTURE(embind);

  auto expectedContains = R"(.constructor<std::vector<int>>())";
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(embind, expectedContains));
}

TEST_CASE("Class with static function", "[classBuilder]") {
  auto s = TestUtil::getStruct("MyStruct");

  IR::Function f;

  f.m_name = "f";
  f.m_representation = f.m_name;
  f.m_isStatic = true;

  f.m_returnType = TestUtil::getType();

  s.m_public.m_functions.push_back(f);

  Embind::Proxy::TypeInfo typeInfo;
  auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
  auto embind = myStruct.getEmbind();
  CAPTURE(embind);

  auto expectedContains =
      fmt::format("\t.class_function(\"{function}\", &{function})",
                  fmt::arg("function", f.m_name));
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(embind, expectedContains));
}

TEST_CASE("Templated class", "[classBuilder]") {
  auto s = TestUtil::getStruct("SomeClass<int>");
  IR::Type t = TestUtil::getType();
  s.m_templateArguments.push_back(t);

  Embind::Proxy::TypeInfo typeInfo;
  auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
  auto embind = myStruct.getEmbind();
  auto expectedContains =
      fmt::format(R"(class_<{fullyQualifiedClassName}>("SomeClass_int"))",
                  fmt::arg("fullyQualifiedClassName", s.m_representation));
  CAPTURE(embind);
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(embind, expectedContains));
}

TEST_CASE("Empty class gets default constructor", "[classBuilder]") {
  IR::Struct s = TestUtil::getStruct("MyStruct");
  s.m_hasImplicitDefaultConstructor = true;

  Embind::Proxy::TypeInfo typeInfo;
  auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
  auto embind = myStruct.getEmbind();
  CAPTURE(embind);

  auto expectedContains = ".constructor<>()";
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(embind, expectedContains));
}

TEST_CASE("Class with functions", "[classBuilder]") {
  IR::Struct s = TestUtil::getStruct("MyStruct");

  std::vector<std::pair<std::string, std::string>> functions = {
      std::make_pair("f", "int"),
      std::make_pair("another_func", "std::string"),
      std::make_pair("fun", "double")};
  for (auto const& [function, type] : functions) {
    IR::Function f = TestUtil::getFunction(function);

    IR::Argument v;
    v.m_name = "myVar";

    IR::Type arg;
    arg.m_representation = type;
    v.m_type = arg;
    f.m_arguments.push_back(v);

    f.m_returnType = TestUtil::getType();

    s.m_public.m_functions.push_back(f);
  }

  Embind::Proxy::TypeInfo typeInfo;
  auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
  auto embind = myStruct.getEmbind();
  CAPTURE(embind);

  for (auto const& [function, argument] : functions) {
    auto expectedContains =
        fmt::format("\t.function(\"{function}\", &{function})",
                    fmt::arg("function", function));
    CAPTURE(expectedContains);
    REQUIRE(TestUtil::contains(embind, expectedContains));
  }
}

TEST_CASE("Class with vector in constructor gives the correct register command",
          "[classBuilder]") {
  IR::Struct s = TestUtil::getStruct("MyStruct");
  IR::Function constructor = TestUtil::getFunction("MyStruct");
  IR::Type v = TestUtil::getVector();
  IR::Argument arg;
  arg.m_name = "myVar";
  arg.m_type = v;
  constructor.m_arguments.push_back(arg);
  s.m_public.m_functions.push_back(constructor);
  Embind::Proxy::TypeInfo typeInfo;
  auto c = Embind::Builders::buildClass(s, typeInfo);
  REQUIRE(c.has_value());
  auto& regs = typeInfo.m_registerCommands;
  REQUIRE(regs.size() == 1);
  REQUIRE(regs.find(R"(em::register_vector<int>("vector_int"))") != regs.end());
}

TEST_CASE("Class with member variables", "[classBuilder]") {
  std::vector<Var> variables = {Var({"v0", "int", true}),
                                Var({"s", "const std::string&", false}),
                                Var({"myVar", "double", false})};

  IR::Struct s = TestUtil::getStruct("MyStruct");

  for (auto const& var : variables) {
    IR::Variable v;
    v.m_name = var.name;
    v.m_isStatic = false;
    v.m_documentation = "";
    v.m_id = 0;
    v.m_type = TestUtil::getType();
    v.m_type.m_representation = var.type;
    v.m_type.m_isConst = var.isConst;
    s.m_public.m_memberVariables.push_back(v);
  }

  Embind::Proxy::TypeInfo typeInfo;
  auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
  auto embind = myStruct.getEmbind();
  CAPTURE(embind);

  for (auto const& var : variables) {
    auto property = fmt::format(R"(.property("{variableName}")",
                                fmt::arg("variableName", var.name));
    CAPTURE(property);
    REQUIRE(TestUtil::contains(embind, property));

    if (var.isConst) {
      auto getter = fmt::format(R"(&{className}::{variableName})",
                                fmt::arg("className", s.m_representation),
                                fmt::arg("variableName", var.name));
      CAPTURE(getter);
      REQUIRE(TestUtil::contains(embind, getter));
    } else {
      auto getter = fmt::format(R"(&{ns}::{className}_get_{variableName})",
                                fmt::arg("ns", typeInfo.m_functionsNamespace),
                                fmt::arg("className", s.m_representation),
                                fmt::arg("variableName", var.name));
      auto setter = fmt::format(R"(&{ns}::{className}_set_{variableName})",
                                fmt::arg("ns", typeInfo.m_functionsNamespace),
                                fmt::arg("className", s.m_representation),
                                fmt::arg("variableName", var.name));
      CAPTURE(getter);
      CAPTURE(setter);
      REQUIRE(TestUtil::contains(embind, getter));
      REQUIRE(TestUtil::contains(embind, setter));
    }
  }
}

// TEST_CASE("Class within namespace", "[classBuilder]") {
// 	IR::Struct s;
// 	s.m_name = "MyStruct";
// 	s.m_representation = fmt::format("MyNamespace::{className}",
// 	                                 fmt::arg("className", s.m_name));
// 	s.m_hasImplicitDefaultConstructor = true;

// 	Embind::Proxy::TypeInfo typeInfo;
// 	auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
// 	auto embind = myStruct.getEmbind();
// 	CAPTURE(embind);

// 	auto expectedContains =
// 	    fmt::format(R"(py::class_<{fullyQualifiedClassName}>("{className}"))",
// 	                fmt::arg("fullyQualifiedClassName", s.m_representation),
// 	                fmt::arg("className", s.m_name));
// 	CAPTURE(expectedContains);
// 	REQUIRE(TestUtil::contains(embind, expectedContains));
// }

// TEST_CASE("Class with enum", "[classBuilder]") {
// 	IR::Struct s;
// 	s.m_name = "SomeClass";
// 	s.m_hasImplicitDefaultConstructor = true;
// 	IR::Enum e;
// 	e.m_isScoped = true;
// 	e.m_name = "MyEnum";
// 	e.m_representation = "::" + e.m_name;
// 	e.m_values.push_back("Hi");
// 	s.m_public.m_enums.push_back(e);

// 	Embind::Proxy::TypeInfo typeInfo;
// 	auto myStruct = Embind::Builders::buildClass(s, typeInfo).value();
// 	auto embind = myStruct.getEmbind();
// 	auto expectedContains = fmt::format(
// 	    R"(py::enum_<{representation}>({structureName}, "{enumName}")",
// 	    fmt::arg("representation", e.m_representation),
// 	    fmt::arg("enumName", e.m_name),
// 	    fmt::arg("structureName", s.m_name));
// 	CAPTURE(embind);
// 	CAPTURE(expectedContains);
// 	REQUIRE(TestUtil::contains(embind, expectedContains));
// }
