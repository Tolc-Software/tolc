#include "Frontend/Objc/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("No name clash", "[libraryNSClash]") {
  std::string moduleName = "m";
  std::string cppCode = R"(
namespace m {
	void f();
}
)";

  auto global = TestUtil::parse(cppCode);
  auto maybeOutput = Frontend::Objc::createModule(global, moduleName);
  // Fine since global namespace is empty
  REQUIRE(maybeOutput.has_value());
}

TEST_CASE("No name clash2", "[libraryNSClash]") {
  std::string moduleName = "m";
  std::string cppCode = R"(
void f();

namespace m {}
)";

  auto global = TestUtil::parse(cppCode);
  auto maybeOutput = Frontend::Objc::createModule(global, moduleName);
  // Fine since global namespace is empty
  REQUIRE(maybeOutput.has_value());
}

TEST_CASE("Name clash with moduleName", "[libraryNSClash]") {
  std::string moduleName = "m";
  std::string cppCode = R"(
void illegal();

namespace m {
	void f();
}
)";

  auto global = TestUtil::parse(cppCode);
  auto maybeOutput = Frontend::Objc::createModule(global, moduleName);
  REQUIRE(!maybeOutput.has_value());
}

TEST_CASE("Base case", "[libraryNSClash]") {
  std::string moduleName = "m";
  std::string cppCode = R"(

namespace m {
}
)";

  auto global = TestUtil::parse(cppCode);
  auto maybeOutput = Frontend::Objc::createModule(global, moduleName);
  REQUIRE(maybeOutput.has_value());
}
