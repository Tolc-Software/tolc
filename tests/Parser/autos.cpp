#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <string>

TEST_CASE("Filesystem::path is parsed", "[paths]") {
  std::string code = R"(
#include <array>
#include <string>

auto getS(std::array<std::string, 2> s) { return s; }
)";
  auto globalNS = TestUtil::parseString(code);
  auto& getS = TestUtil::findFunction(globalNS, "getS");
  auto& array = getS.m_returnType;
  TestUtil::compare(array, IR::ContainerType::Array);
}
