#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("You can refer to things as long as they are previously defined",
          "[dependency]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
namespace MyLib {
struct Data { int i; };
}

MyLib::Data complexFunction() {
	MyLib::Data d;
	d.i = 5;
	return d;
}
)";

	auto pythonTestCode = fmt::format(R"(
# Can call a function which depends on an inner object
data = {moduleName}.complexFunction()
self.assertEqual(data.i, 5)
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);
}
