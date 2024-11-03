#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("std::complex gets converted to complex", "[complex]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <complex>

using namespace std::complex_literals;

std::complex<int> i() {
	return 5;
}

std::complex<double> d() {
	return 1. + 2i;
}

std::complex<float> f() {
	return 0.f + 5if;
}

std::complex<double> r(std::complex<double> d) {
	return d;
}
)";

	auto pythonTestCode = fmt::format(R"(
# std::complex translates to a complex in python
i = {moduleName}.i()
self.assertEqual(i.real, 5)
self.assertEqual(i.imag, 0)

d = {moduleName}.d()
self.assertEqual(d.real, 1)
self.assertEqual(d.imag, 2)

f = {moduleName}.f()
self.assertEqual(f.real, 0)
self.assertEqual(f.imag, 5)

# Using python builtin complex class
r = {moduleName}.r(complex(1, 2))
self.assertEqual(r.real, 1)
self.assertEqual(r.imag, 2)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("std::complex");
}
