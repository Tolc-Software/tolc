#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <Parser/Parse.hpp>
#include <TestUtil/parserConfig.hpp>
#include <catch2/catch_test_macros.hpp>

#include <optional>
#include <string>

namespace TestUtil {

IR::Namespace parseString(std::string const& code) {
  auto ns = Parser::parseString(code, TestUtil::getParserConfig());
  REQUIRE(ns.has_value());
  return ns.has_value() ? ns.value() : IR::Namespace();
}

IR::Namespace parseFile(std::string const& filepath) {
  auto ns = Parser::parseFile(filepath, TestUtil::getParserConfig());
  REQUIRE(ns.has_value());
  return ns.has_value() ? ns.value() : IR::Namespace();
}
}    // namespace TestUtil
