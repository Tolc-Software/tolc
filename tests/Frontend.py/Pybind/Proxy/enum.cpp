#include "Pybind/Proxy/enum.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Scoped and unscoped enums", "[enum]") {
	std::string moduleName = "MyModule";
	std::string name = "MyEnum";
	std::string fullyQualifiedName = moduleName + "::" + name;
	Pybind::Proxy::Enum e(name, fullyQualifiedName);
	std::string value = "MyValue";
	e.addValue(value);
	for (bool isScoped : {true, false}) {
		e.setScoped(isScoped);
		auto pybind = e.getPybind(moduleName);

		auto expectedContains = fmt::format(
		    R"(py::enum_<{fullyQualifiedName}>({moduleName}, "{name}")",
		    fmt::arg("fullyQualifiedName", fullyQualifiedName),
		    fmt::arg("name", name),
		    fmt::arg("moduleName", moduleName));

		auto valueExists =
		    fmt::format(R"(.value("{value}", {fullyQualifiedName}::{value}))",
		                fmt::arg("value", value),
		                fmt::arg("fullyQualifiedName", fullyQualifiedName));

		CAPTURE(pybind);
		CAPTURE(expectedContains);
		REQUIRE(TestUtil::contains(pybind, expectedContains));

		CAPTURE(valueExists);
		REQUIRE(TestUtil::contains(pybind, valueExists));

		if (isScoped) {
			REQUIRE(TestUtil::contains(pybind, "py::arithmetic()"));
		} else {
			// Exporting values to the parent namespace to make it behave more like C++
			REQUIRE(TestUtil::contains(pybind, ".export_values()"));
		}
	}
}
