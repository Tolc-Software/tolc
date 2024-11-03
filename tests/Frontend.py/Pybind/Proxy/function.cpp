#include "Pybind/Proxy/function.hpp"
#include "Pybind/returnValuePolicy.hpp"
#include "TestUtil/string.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>
#include <vector>

TEST_CASE("Static global function", "[function]") {
	Pybind::Proxy::Function f("f", "MyNamespace::f");
	f.setAsStatic();

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	REQUIRE(
	    TestUtil::contains(pybindCode, R"(def_static("f", &MyNamespace::f)"));
}

TEST_CASE("Test signature if no overloads", "[function]") {
	Pybind::Proxy::Function f("f", "f");
	f.setDocumentation("This is a function");
	std::vector<std::string> args = {"i", "j", "k"};
	for (auto const& arg : args) {
		f.addArgument("int", arg);
	}

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);
	std::string expectedContains =
	    "def(\"f\", &f, R\"_tolc_docs(This is a function)_tolc_docs\"";
	CAPTURE(expectedContains);

	// The function bind does not contain the signature since it is not overloaded
	REQUIRE(TestUtil::contains(pybindCode, expectedContains));
}

TEST_CASE("Test signature of overloaded function", "[function]") {
	Pybind::Proxy::Function f("f", "f");
	std::vector<std::string> args = {"i", "j", "k"};
	for (auto const& arg : args) {
		f.addArgument("int", arg);
	}
	f.setAsOverloaded();

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	// The function bind contains the signature since it is overloaded (artificially)
	REQUIRE(TestUtil::contains(pybindCode, R"((void(*)(int, int, int))"));
}

TEST_CASE("Simple function within a namespace", "[function]") {
	Pybind::Proxy::Function f("f", "MyNamespace::f");

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	// The function bind
	REQUIRE(TestUtil::contains(pybindCode, R"(def("f", &MyNamespace::f)"));
}

TEST_CASE("Function with arguments", "[function]") {
	Pybind::Proxy::Function f("f", "f");
	std::vector<std::string> args = {"i", "j", "k"};
	for (auto const& arg : args) {
		f.addArgument("int", arg);
	}

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	// The function bind
	REQUIRE(TestUtil::contains(pybindCode, R"(def("f", &f)"));

	for (auto const& arg : args) {
		REQUIRE(TestUtil::contains(pybindCode,
		                           fmt::format(R"(py::arg("{}"))", arg)));
	}
}

TEST_CASE("Function with arguments that are not named", "[function]") {
	Pybind::Proxy::Function f("f", "f");
	std::vector<std::string> args = {"i", "", "k"};
	//                                    ^ anonymous argument
	// Ex: int f(int i, int, int k);
	for (auto const& arg : args) {
		f.addArgument("int", arg);
	}

	auto pybindCode = f.getPybind();
	CAPTURE(pybindCode);

	// The function bind
	REQUIRE(TestUtil::contains(pybindCode, R"(def("f", &f)"));

	for (auto const& arg : args) {
		// Should not contain any of the arguments
		REQUIRE(!TestUtil::contains(pybindCode,
		                            fmt::format(R"(py::arg("{}"))", arg)));
	}
}

TEST_CASE("Return value policy", "[function]") {
	using ReturnValue = Pybind::Proxy::Function::return_value_policy;
	std::vector<ReturnValue> policies = {ReturnValue::automatic,
	                                     ReturnValue::take_ownership,
	                                     ReturnValue::copy,
	                                     ReturnValue::move,
	                                     ReturnValue::reference,
	                                     ReturnValue::reference_internal,
	                                     ReturnValue::automatic,
	                                     ReturnValue::automatic_reference};

	for (auto const& policy : policies) {
		Pybind::Proxy::Function f("f", "f");
		f.setReturnValuePolicy(policy);

		auto pybindCode = f.getPybind();
		auto containsString = fmt::format(
		    R"(, py::{})", Pybind::returnValuePolicyToString(policy));

		CAPTURE(pybindCode);
		CAPTURE(containsString);

		REQUIRE(TestUtil::contains(pybindCode, containsString));
	}
}
