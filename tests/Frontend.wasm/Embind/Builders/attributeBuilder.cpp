#include "Embind/Builders/attributeBuilder.hpp"
#include "TestUtil/string.hpp"
#include "TestUtil/types.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Can build simple attributes", "[attributeBuilder]") {
  IR::Variable v;

  std::string variableName = "i";
  v.m_name = variableName;

  std::string moduleName = "Module";
  auto fullyQualifiedName = moduleName + "::" + variableName;

  Embind::Proxy::TypeInfo typeInfo;
  auto proxyAttribute =
      Embind::Builders::buildAttribute(moduleName, v, typeInfo);
  auto embind = proxyAttribute.getEmbind();
  CAPTURE(embind);

  auto expectedContains =
      fmt::format(R"(em::constant("{name}", {fullyQualifiedName}))",
                  fmt::arg("fullyQualifiedName", fullyQualifiedName),
                  fmt::arg("name", v.m_name));
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(embind, expectedContains));
}

TEST_CASE("String attributes", "[attributeBuilder]") {
  using IR::BaseType;
  for (auto const& t :
       {BaseType::String, BaseType::StringView, BaseType::Char}) {
    IR::Variable v;

    std::string variableName = "i";
    v.m_name = variableName;
    v.m_type = TestUtil::getType(t);
    if (t == BaseType::Char) {
      v.m_type.m_numPointers = 1;
    }

    std::string moduleName = "Module";

    Embind::Proxy::TypeInfo typeInfo;
    auto proxyAttribute =
        Embind::Builders::buildAttribute(moduleName, v, typeInfo);
    auto embind = proxyAttribute.getEmbind();
    CAPTURE(embind);

    auto callName = moduleName + "::" + variableName;
    if (t == BaseType::StringView) {
      callName = fmt::format("em::val({}.data())", callName);
      REQUIRE(typeInfo.m_includes.size() == 1);
      REQUIRE(typeInfo.m_includes.find("<emscripten/val.h>") !=
              typeInfo.m_includes.end());
    } else if (t == BaseType::Char) {
      callName = fmt::format("em::val({})", callName);
      REQUIRE(typeInfo.m_includes.size() == 1);
      REQUIRE(typeInfo.m_includes.find("<emscripten/val.h>") !=
              typeInfo.m_includes.end());
    }
    auto expectedContains = fmt::format(R"(em::constant("{name}", {callName}))",
                                        fmt::arg("callName", callName),
                                        fmt::arg("name", v.m_name));
    CAPTURE(expectedContains);
    REQUIRE(TestUtil::contains(embind, expectedContains));
  }
}
