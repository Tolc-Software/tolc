#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <string>
#include <utility>
#include <variant>

TEST_CASE("Function that returns base types", "[functions]") {
  // String requires an include
  for (auto baseType : TestUtil::getBaseTypes({"std::string"})) {
    auto globalNS = TestUtil::parseString(baseType + " fun();");
    REQUIRE(globalNS.m_functions.size() == 1);
    auto fun = globalNS.m_functions[0];
    CHECK(fun.m_name == "fun");
    CHECK(fun.m_arguments.size() == 0);
    auto irType = TestUtil::getIRFromString(baseType);
    REQUIRE(irType.has_value());
    TestUtil::compare(fun.m_returnType, irType.value());
  }
}

TEST_CASE("Function works with default modifier", "[functions]") {
  using TestUtil::AccessModifier;
  for (auto [accessModifier, structure] :
       {std::make_pair(AccessModifier::Private, std::string("class")),
        std::make_pair(AccessModifier::Public, std::string("struct"))}) {
    auto globalNS =
        TestUtil::parseString(structure + " MyStructure { void fun(); };");

    REQUIRE(globalNS.m_functions.size() == 0);
    REQUIRE(globalNS.m_structs.size() == 1);
    auto s = globalNS.m_structs[0];
    auto& functions = accessModifier == AccessModifier::Private ?
                          s.m_private.m_functions :
                          s.m_public.m_functions;
    REQUIRE(functions.size() == 1);
    auto& fun = functions.back();
    CHECK(fun.m_name == "fun");
  }
}

TEST_CASE("Function within class with modifier", "[functions]") {
  for (auto accessModifier : TestUtil::getAccessModifiers()) {
    auto globalNS = TestUtil::parseString(
        "class MyClass { " + TestUtil::getAsString(accessModifier) +
        ": void fun(); };");
    REQUIRE(globalNS.m_functions.size() == 0);
    REQUIRE(globalNS.m_structs.size() == 1);
    auto myClass = globalNS.m_structs[0];
    auto data = TestUtil::getStructDataBasedOnAccess(myClass, accessModifier);
    REQUIRE(data != nullptr);
    REQUIRE(data->m_functions.size() == 1);
    auto& fun = data->m_functions.back();
    CHECK(fun.m_name == "fun");
  }
}

TEST_CASE("Function within namespace", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
namespace NS {
void fun();
}
		)");
  REQUIRE(globalNS.m_functions.size() == 0);
  REQUIRE(globalNS.m_namespaces.size() == 1);
  auto NS = globalNS.m_namespaces[0];
  REQUIRE(NS.m_functions.size() == 1);
  auto& fun = NS.m_functions.back();
  CHECK(fun.m_name == "fun");
  CHECK(fun.m_arguments.size() == 0);
}

TEST_CASE("Function within class", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
class MyClass {
void fun();
};
		)");
  REQUIRE(globalNS.m_functions.size() == 0);
  REQUIRE(globalNS.m_structs.size() == 1);
  auto myClass = globalNS.m_structs[0];
  REQUIRE(myClass.m_private.m_functions.size() == 1);
  auto& fun = myClass.m_private.m_functions.back();
  CHECK(fun.m_name == "fun");
  CHECK(fun.m_arguments.size() == 0);
}

TEST_CASE("Simple function", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
void fun() {}
		)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto fun = globalNS.m_functions[0];
  CHECK(fun.m_name == "fun");
  CHECK(fun.m_arguments.size() == 0);
  TestUtil::compare(fun.m_returnType, IR::BaseType::Void);
}

TEST_CASE("Function with different returns without includes", "[functions]") {
  for (auto irType : TestUtil::getTypes()) {
    // Remove the ones who require an include
    if (auto include = TestUtil::getIncludesIfNeeded(irType); include.empty()) {
      auto type = TestUtil::getAsString(irType);
      std::string code = include + type + " fun() { return " +
                         TestUtil::getValidReturnForType(irType) + "; }";
      auto globalNS = TestUtil::parseString(code);
      // Print on error
      CAPTURE(code);
      CAPTURE(type);

      REQUIRE(globalNS.m_functions.size() == 1);
      auto fun = globalNS.m_functions[0];
      CHECK(fun.m_name == "fun");
      CHECK(fun.m_arguments.size() == 0);
      TestUtil::compare(fun.m_returnType, irType);
    }
  }
}

TEST_CASE("Function with arguments not requiring includes", "[functions]") {
  for (auto irType : TestUtil::getTypes()) {
    // Remove the ones who require an include
    if (auto include = TestUtil::getIncludesIfNeeded(irType); include.empty()) {
      auto type = TestUtil::getAsString(irType);
      if (type == "void") {
        // Named variables cannot have type void
        continue;
      }
      std::string code = "void fun(" + type + " myArg) { return; }";
      auto globalNS = TestUtil::parseString(code);
      // Print on error
      CAPTURE(code);
      CAPTURE(type);

      REQUIRE(globalNS.m_functions.size() == 1);
      auto fun = globalNS.m_functions[0];
      CHECK(fun.m_name == "fun");
      TestUtil::compare(fun.m_returnType, IR::BaseType::Void);

      REQUIRE(fun.m_arguments.size() == 1);
      auto& arg = fun.m_arguments.back();
      CHECK(arg.m_name == "myArg");
      TestUtil::compare(arg.m_type, irType);
    }
  }
}

TEST_CASE("Multiple arguments", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
void fun(int i, double d, char c);
		)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& fun = globalNS.m_functions.back();
  REQUIRE(fun.m_arguments.size() == 3);
  for (auto [argName, type] : {std::make_pair("i", IR::BaseType::Int),
                               std::make_pair("d", IR::BaseType::Double),
                               std::make_pair("c", IR::BaseType::Char)}) {
    auto maybeArg = TestUtil::findWithName(argName, fun.m_arguments);
    REQUIRE(maybeArg.has_value());
    auto arg = maybeArg.value();
    TestUtil::compare(arg.m_type, type);
  }
}

TEST_CASE("Function with const argument", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
void fun(int const i) {}
		)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto fun = globalNS.m_functions[0];
  REQUIRE(fun.m_arguments.size() == 1);
  auto& arg = fun.m_arguments.back();
  REQUIRE(arg.m_type.m_isConst);
}

TEST_CASE("Function with pointer argument", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
void fun(int* i);
		)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto fun = globalNS.m_functions[0];
  REQUIRE(fun.m_arguments.size() == 1);
  auto& arg = fun.m_arguments.back();
  REQUIRE(arg.m_type.m_numPointers == 1);
}

TEST_CASE("Function with const pointer argument", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
void fun(int const* i);
		)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto fun = globalNS.m_functions[0];
  REQUIRE(fun.m_arguments.size() == 1);
  auto& arg = fun.m_arguments.back();
  REQUIRE(arg.m_type.m_numPointers == 1);
  REQUIRE(arg.m_type.m_isConst);
}

TEST_CASE("Function with const return value", "[functions]") {
  auto globalNS = TestUtil::parseString(R"(
char const* fun();
		)");
  REQUIRE(globalNS.m_functions.size() == 1);
  auto fun = globalNS.m_functions[0];
  auto returnChar = std::get_if<IR::Type::Value>(&fun.m_returnType.m_type);
  REQUIRE(returnChar);
  CHECK(returnChar->m_base == IR::BaseType::Char);
  CHECK(fun.m_returnType.m_numPointers == 1);
  REQUIRE(fun.m_returnType.m_isConst);
}
