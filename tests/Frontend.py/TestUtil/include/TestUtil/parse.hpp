#pragma once

#include "TestUtil/parserConfig.hpp"

#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string>

namespace TestUtil {

IR::Namespace parseString(std::string const& code) {
	auto parsed = Parser::parseString(code, TestUtil::getParserConfig());
	REQUIRE(parsed.has_value());
	return parsed.value();
}

IR::Namespace parseFile(std::string const& filepath) {
	auto parsed = Parser::parseFile(filepath, TestUtil::getParserConfig());
	REQUIRE(parsed.has_value());
	return parsed.value();
}
}    // namespace TestUtil
