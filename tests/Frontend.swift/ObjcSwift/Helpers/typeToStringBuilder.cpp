#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Types to string", "[typeToStringBuilder]") {
  auto ns = TestUtil::parse(R"(
#include <vector>

struct Base {
void f(std::vector<double> const& v);
};)");

  auto base = TestUtil::findStruct(ns, "Base");
  auto f = TestUtil::findFunction(base, "f", TestUtil::AccessModifier::Public);
  REQUIRE(f.m_arguments.size() == 1);
  auto typeString =
      ObjcSwift::Helpers::buildTypeString(f.m_arguments.back().m_type);
  REQUIRE(typeString == "VectorDouble");
}
