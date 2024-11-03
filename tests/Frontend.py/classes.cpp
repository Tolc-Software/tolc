#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Classes", "[classes]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);
	// Add instantiation in a source file.
	// This cannot be just declared, must be instantiated
	// See https://github.com/pybind/pybind11/issues/682
	// And https://en.cppreference.com/w/cpp/language/static
	//
	// Instantiation (must be in a source file):
	stage.addModuleFile("test.cpp", "int const WithConstructor::i;");

	auto cppCode = R"(
#include <string>
#include <string_view>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	static int const i = 5;

	// This class has a readwrite variable
	int readwrite = 10;

	std::string getS() { return m_s; }
	std::string_view getSView() { return m_s; }

private:
	std::string m_s;
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};

class WithPrivateFunction {
	double multiply(double i, double j) {
		return i * j;
	}
};

namespace MyLib {

	class Nested {
	public:
		double divideByTwo(double d) {
			return d / 2;
		}
	};
}

/** Documentation carries over */
struct Documentation {};

/*****************************
* JavaDoc Style
* is
* boxy
****************************/
struct JavaDoc {};
)";

	auto pythonTestCode = fmt::format(R"(
# You can access static variables without instantiating class
self.assertEqual({moduleName}.WithConstructor.i, 5)

# Creating classes via their constructor
with_constructor = {moduleName}.WithConstructor("Hello")
self.assertEqual(with_constructor.getS(), "Hello")

# Documentation for variables carries over aswell
self.assertIn("This class has a readwrite variable", {moduleName}.WithConstructor.readwrite.__doc__)

# Named arguments in constructors
with_constructor = {moduleName}.WithConstructor(s="named argument")
self.assertEqual(with_constructor.getS(), "named argument")
self.assertEqual(with_constructor.getSView(), "named argument")

# Member functions are available after construction
with_function = {moduleName}.WithFunction()
self.assertEqual(with_function.add(2, 5), 7)

# Private functions have no bindings
with self.assertRaises(AttributeError) as error_context:
    with_private_function = {moduleName}.WithPrivateFunction()
    with_private_function.multiply(3, 2)

self.assertEqual(len(error_context.exception.args), 1)
# print(error_context.test_case)
self.assertEqual(
    "'{moduleName}.WithPrivateFunction' object has no attribute 'multiply'",
    error_context.exception.args[0],
    "Not correct exception on private functions",
)

# Classes under namespaces are available under the corresponding submodule
nested = {moduleName}.MyLib.Nested()
self.assertEqual(nested.divideByTwo(10), 5)

# Different styles of documentation on classes carries over
self.assertIn("Documentation carries over", {moduleName}.Documentation.__doc__)
self.assertIn("JavaDoc Style", {moduleName}.JavaDoc.__doc__)

)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Classes");
}
