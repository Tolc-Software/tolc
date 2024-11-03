#include "TestUtil/parse.hpp"
#include "TestUtil/parserConfig.hpp"

#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string>

namespace TestUtil {
IR::Namespace parse(std::string code) {
  auto parsed = Parser::parseString(code, TestUtil::getParserConfig());
  REQUIRE(parsed.has_value());
  return parsed.value();
}

std::optional<IR::Namespace> parseUnsafe(std::string code) {
  return Parser::parseString(code, TestUtil::getParserConfig());
}

}    // namespace TestUtil
