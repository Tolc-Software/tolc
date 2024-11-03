#include "Helpers/commandLineArgs.hpp"
#include "Parser/Config.hpp"

#include <catch2/catch_test_macros.hpp>

#include <filesystem>
#include <string>

namespace {
std::string removeSubstring(std::string str, std::string const& subString) {
  // Search for the include flag substring in string
  auto pos = str.find(subString);

  if (pos != std::string::npos) {
    // If found then erase it from string
    str.erase(pos, subString.length());
  }
  return str;
}
}    // namespace

TEST_CASE("getCommandLineArgs returns a custom system include path",
          "[commandLineArgs]") {
  using path = std::filesystem::path;
  std::string myPath = (path("my") / path("custom") / path("path")).string();
  std::string systemInclude = "-isystem";
  Parser::Config c;
  c.m_systemIncludes = {systemInclude + myPath};
  auto args = Helpers::getCommandLineArgs(c);
  REQUIRE(!args.empty());
  for (auto arg : args) {
    CAPTURE(arg);
    if (auto include = removeSubstring(arg, systemInclude); include != arg) {
      CAPTURE(include);
      REQUIRE(include == myPath);
    }
  }
}

TEST_CASE("Sets the correct standard", "[commandLineArgs]") {
  Parser::Config c;
  c.m_cppVersion = 20;
  auto args = Helpers::getCommandLineArgs(c);
  REQUIRE(!args.empty());
  bool foundStandard = false;
  for (auto arg : args) {
    CAPTURE(arg);
    if (arg == "-std=c++20") {
      foundStandard = true;
    }
  }
  REQUIRE(foundStandard);
}
