#include "Embind/Builders/functionBuilder.hpp"

#include <TestUtil/types.hpp>
#include <catch2/catch_test_macros.hpp>

#include <optional>

TEST_CASE("Can build simple functions", "[functionBuilder]") {
  IR::Function f = TestUtil::getFunction("f");
  IR::Type t = TestUtil::getVector();
  f.m_returnType = t;
  Embind::Proxy::TypeInfo typeInfo;
  auto jsFunction = Embind::Builders::buildFunction(f, typeInfo);
  REQUIRE(jsFunction.has_value());
  // The vector is registered
  REQUIRE(typeInfo.m_registerCommands.size() == 1);
}
