#pragma once

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <variant>

namespace TestUtil {

void compare(IR::Type returnType, IR::BaseType compareType) {
	auto valueType = std::get_if<IR::Type::Value>(&returnType.m_type);
	REQUIRE(valueType);
	CHECK(valueType->m_valueType == compareType);
}
}    // namespace TestUtil
