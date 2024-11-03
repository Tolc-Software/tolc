#include "Pybind/Builders/enumBuilder.hpp"
#include "TestUtil/string.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Can build simple enums", "[enumBuilder]") {
	IR::Enum e;

	std::string enumName = "MyEnum";
	e.m_name = enumName;

	std::string moduleName = "Module";
	auto fullyQualifiedName = moduleName + "::" + enumName;
	e.m_representation = fullyQualifiedName;

	std::vector<std::string> values = {"A", "B", "Ending"};
	e.m_values = values;

	e.m_isScoped = true;

	auto proxyEnum = Pybind::Builders::buildEnum(e);
	auto pybind = proxyEnum.getPybind(moduleName);
	CAPTURE(pybind);

	// Since it is an enum class we should not export it
	REQUIRE(!TestUtil::contains(pybind, "export_values()"));

	auto expectedContains = fmt::format(
	    R"(py::enum_<{fullyQualifiedName}>({moduleName}, "{enumName}")",
	    fmt::arg("fullyQualifiedName", e.m_representation),
	    fmt::arg("moduleName", moduleName),
	    fmt::arg("enumName", e.m_name));
	CAPTURE(expectedContains);
	REQUIRE(TestUtil::contains(pybind, expectedContains));
	for (auto const& value : values) {
		CAPTURE(value);
		// .value("A", Module::MyEnum::A)
		REQUIRE(TestUtil::contains(
		    pybind,
		    fmt::format(R"(.value("{value}", {fullyQualifiedName}::{value})",
		                fmt::arg("value", value),
		                fmt::arg("fullyQualifiedName", e.m_representation))));
	}
}
