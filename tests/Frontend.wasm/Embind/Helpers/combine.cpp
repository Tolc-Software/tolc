#include "Embind/Helpers/combine.hpp"

#include <catch2/catch_test_macros.hpp>

#include <set>
#include <string>
#include <vector>

TEST_CASE("Combine some strings", "[combine]") {
  std::vector<std::string> v = {
      "<string>", "<string>", "\"Embind/Helpers/combine.hpp\""};
  std::set<std::string> s;
  Embind::Helpers::combine(s, v);
  REQUIRE(v.size() == 3);
  REQUIRE(s.size() == 2);
  for (auto value : {"<string>", "\"Embind/Helpers/combine.hpp\""}) {
    REQUIRE(s.find(value) != s.end());
  }
}
