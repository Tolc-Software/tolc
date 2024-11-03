#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Namespaces turn into modules", "[namespaces]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

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

  auto jsTestCode = R"(
expect(m.MyLib.complexFunction()).toBe(5);

// Namespaces can be nested arbitrarily
expect(m.MyLib.We.Are.Going.Pretty.Deep.meaningOfLife()).toBe('42');
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Namespaces");
}
