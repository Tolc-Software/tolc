#include "Helpers/Utils/string.hpp"

#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Removing substrings", "[string]") {
  using Helpers::Utils::removeSubString;
  {
    std::string s = "Hello struct Yes";
    std::string substr = "struct ";
    REQUIRE(removeSubString(s, substr) == "Hello Yes");
  }

  {
    std::string s = "Hello Yes";
    std::string substr = "struct ";
    REQUIRE(removeSubString(s, substr) == "Hello Yes");
  }
}
