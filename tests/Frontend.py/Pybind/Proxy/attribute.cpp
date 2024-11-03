#include "Pybind/Proxy/attribute.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Simple attribute", "[attribute]") {
	std::string fqName = "Stuff::i";
	std::string name = "i";
	Pybind::Proxy::Attribute a(name, fqName);
	auto pybindCode = a.getPybind();
	CAPTURE(pybindCode);

	auto expectedContains = fmt::format(R"(attr("{name}") = &{fqName})",
	                                    fmt::arg("name", name),
	                                    fmt::arg("fqName", fqName));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybindCode, expectedContains));
}
