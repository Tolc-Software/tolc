#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Namespaces turn into modules", "[namespaces]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

/*
* MyLib contains a bunch of MyLib functions
*/
namespace MyLib {

int complexFunction() {
	return 5;
}

	namespace We {
		namespace Are {
			namespace Going {
				namespace Pretty {
					namespace Deep {
						std::string meaningOfLife() {
							return "42";
						}
					}
				}
			}
		}
	}
}

)";

	auto pythonTestCode = fmt::format(R"(
# Namespaces corresponds to submodules
result = {moduleName}.MyLib.complexFunction()
self.assertEqual(result, 5)

# Documentation carries over for namespaces
self.assertIn("MyLib contains a bunch of MyLib functions", \
  {moduleName}.MyLib.__doc__)

# You can nest namespaces arbitrarily deep
lifeProTips = {moduleName}.MyLib.We.Are.Going.Pretty.Deep.meaningOfLife()
self.assertEqual(lifeProTips, "42")
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Namespaces");
}
