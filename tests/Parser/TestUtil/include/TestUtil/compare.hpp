#pragma once

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <variant>

namespace TestUtil {

void compare(IR::Type type, IR::BaseType compareType) {
  auto valueType = std::get_if<IR::Type::Value>(&type.m_type);
  REQUIRE(valueType);
  REQUIRE(valueType->m_base == compareType);
}

void compare(IR::Type type, IR::ContainerType compareType) {
  auto valueType = std::get_if<IR::Type::Container>(&type.m_type);
  REQUIRE(valueType);
  REQUIRE(valueType->m_container == compareType);
}
}    // namespace TestUtil
