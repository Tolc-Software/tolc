#include "ObjcSwift/Builders/functionBuilder.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("function fails on unique_ptr argument", "[functionBuilder]") {
  IR::Function f;
  f.m_representation = "MyNamespace::f";
  IR::Type t;
  IR::Type::Container c;
  c.m_container = IR::ContainerType::UniquePtr;
  t.m_type = c;
  IR::Variable v;
  v.m_type = t;
  v.m_name = "v";
  f.m_arguments.push_back(v);
  auto pyFunction = ObjcSwift::Builders::buildFunction(f);
  REQUIRE(!pyFunction.has_value());
}
