#include "Embind/Helpers/string.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Trim strings", "[strings]") {
  std::string s = "std::vector<int>";
  auto leftTrimmed = Embind::Helpers::trimLeft(s, '<');
  REQUIRE(leftTrimmed == "int>");
  auto rightTrimmed = Embind::Helpers::trimRight(s, '>');
  REQUIRE(rightTrimmed == "std::vector<int");
  REQUIRE(Embind::Helpers::trimLeft(rightTrimmed, '<') == "int");
}
