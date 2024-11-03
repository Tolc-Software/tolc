#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Smart pointers of custom types work", "[smartPointers]") {
	// NOTE: Smart pointers only work with custom defined types.
	//       This makes unique_ptr<int> not computable.
	//       If this is desirable in the future,
	//       wrap the int in a lambda and copy the value
	// Ex:
	//
	//   std::unique_ptr<int> create_int() {
	//      return std::unique_ptr<int>(new int(53));
	//   }
	//
	//   m.def("create_int", []() { return *create_int().get(); });
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <memory>

struct Example {
	int m_hi = 5;
};

struct ExampleShared {
	int m_hi = 10;
};

std::unique_ptr<Example> create_unique() {
	return std::make_unique<Example>();
}

std::shared_ptr<ExampleShared> create_shared() {
	return std::make_shared<ExampleShared>();
}
)";

	auto pythonTestCode = fmt::format(R"(
# std::unique_ptr acts as a normal value
# Note that passing a std::unique_ptr as an argument gives an error
#   See https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html
u = {moduleName}.create_unique()
self.assertEqual(u.m_hi, 5)

# std::shared_ptr acts as a normal value
s = {moduleName}.create_shared()
self.assertEqual(s.m_hi, 10)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Smart Pointers");
}
