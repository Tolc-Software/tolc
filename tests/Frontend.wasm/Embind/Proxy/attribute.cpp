#include "Embind/Proxy/attribute.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Simple attribute", "[attribute]") {
  std::string fqName = "Stuff::i";
  std::string name = "i";
  Embind::Proxy::Attribute a(name, fqName);
  auto pybindCode = a.getEmbind();
  CAPTURE(pybindCode);

  auto expectedContains = fmt::format(R"(em::constant("{name}", {fqName}))",
                                      fmt::arg("name", name),
                                      fmt::arg("fqName", fqName));
  CAPTURE(expectedContains);
  REQUIRE(TestUtil::contains(pybindCode, expectedContains));
}
