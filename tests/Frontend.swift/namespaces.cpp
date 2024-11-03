#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Namespaces turn into modules", "[namespaces]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

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

  [[maybe_unused]] auto objcTestCode = R"(
// Namespaces corresponds to classes
// with {library name} + join(namespaces)
// where functions are static class functions
assert([mMyLib complexFunction] == 5);

// You can nest namespaces arbitrarily deep
NSString* lifeProTips = [mMyLibWeAreGoingPrettyDeep meaningOfLife];
assert([lifeProTips isEqualToString:@"42"]);
)";

  [[maybe_unused]] auto swiftTestCode = R"(
// Namespaces corresponds to classes
// with {library name} + join(namespaces, '.')
// where free functions are static class functions
assert(m.MyLib.complexFunction() == 5);

// You can nest namespaces arbitrarily deep
var lifeProTips: String = m.MyLib.We.Are.Going.Pretty.Deep.meaningOfLife();
assert(lifeProTips == "42");
)";

  stage.keepAliveAfterTest();
  // REQUIRE(stage.runTest(cppCode, objcTestCode, "objc") == 0);
  REQUIRE(stage.runTest(cppCode, swiftTestCode, "swift") == 0);

  stage.exportAsExample("Namespaces");
}
