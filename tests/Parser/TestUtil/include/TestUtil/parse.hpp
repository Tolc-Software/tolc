#pragma once

#include "Parser/Parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <optional>
#include <string>

namespace TestUtil {

IR::Namespace
parseString(std::string const& code,
            spdlog::level::level_enum logLevel = spdlog::level::warn) {
  spdlog::set_level(logLevel);
  auto maybeNs = Parser::parseString(code);
  REQUIRE(maybeNs.has_value());
  return maybeNs.value();
}

IR::Namespace
parseFile(std::filesystem::path const& filepath,
          spdlog::level::level_enum logLevel = spdlog::level::warn) {
  spdlog::set_level(logLevel);
  auto maybeNs = Parser::parseFile(filepath);
  REQUIRE(maybeNs.has_value());
  return maybeNs.value();
}
}    // namespace TestUtil
