#include "IRProxy/IRData.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/getName.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>
#include <utility>

TEST_CASE("Simple root representation", "[representation]") {
  using IRProxy::Structure;
  std::string expected = "Simple";
  {
    auto root = TestUtil::parseString("namespace Simple {}");
    auto& simple = TestUtil::findNamespace(root, "Simple");
    REQUIRE(simple.m_representation == expected);
  }

  {
    auto root = TestUtil::parseString("class Simple {};");
    auto& simple = TestUtil::findStruct(root, "Simple");
    REQUIRE(simple.m_representation == expected);
  }

  {
    auto root = TestUtil::parseString("int Simple();");
    auto& simple = TestUtil::findFunction(root, "Simple");
    REQUIRE(simple.m_representation == expected);
  }
}

TEST_CASE("Representation one namespace deep", "[representation]") {
  using IRProxy::Structure;
  for (auto const& [structure, type] :
       {std::make_pair("namespace Simple {}", Structure::Namespace),
        std::make_pair("class Simple {};", Structure::Struct),
        std::make_pair("int Simple();", Structure::Function)}) {
    auto code = "namespace OuterNamespace { " + std::string(structure) + " }";
    auto globalNS = TestUtil::parseString(code);
    CAPTURE(code);
    REQUIRE(globalNS.m_namespaces.size() == 1);
    auto& inner = globalNS.m_namespaces.back();
    auto [name, representation] =
        TestUtil::getNameAndRepresentation(type, inner);
    REQUIRE("OuterNamespace::Simple" == representation);
  }
}

TEST_CASE("Representation one struct deep", "[representation]") {
  using IRProxy::Structure;
  // NOTE: A class cannot contain a namespace
  for (auto const& [structure, type] :
       {std::make_pair("class Simple {};", Structure::Struct),
        std::make_pair("int Simple();", Structure::Function)}) {
    auto code = "class OuterClass { " + std::string(structure) + " };";
    auto globalNS = TestUtil::parseString(code);
    CAPTURE(code);
    REQUIRE(globalNS.m_structs.size() == 1);
    auto& inner = globalNS.m_structs.back();
    std::string representation = "";
    if (inner.m_private.m_structs.size() == 1) {
      representation = inner.m_private.m_structs.back().m_representation;
    } else if (inner.m_private.m_functions.size() == 1) {
      representation = inner.m_private.m_functions.back().m_representation;
    }
    REQUIRE("OuterClass::Simple" == representation);
  }
}

TEST_CASE("Simple variable representation", "[representation]") {
  using IR::BaseType;
  for (auto const& [typeRepresentation, type] :
       {std::make_pair("int", BaseType::Int),
        std::make_pair("const int", BaseType::Int),
        std::make_pair("const int *", BaseType::Int),
        std::make_pair("std::string", BaseType::String)}) {
    auto code = R"(
#include <string>

class MyClass {)" +
                std::string(typeRepresentation) + " i; };";
    auto globalNS = TestUtil::parseString(code);

    CAPTURE(code);
    auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
    auto& i =
        TestUtil::findMember(myClass, "i", TestUtil::AccessModifier::Private);
    CHECK(i.m_type.m_representation == typeRepresentation);
  }
}
