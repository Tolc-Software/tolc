#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Renaming module objects so they match the C++ more closely",
          "[namespaceRenaming]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

namespace MyNamespace {
	int add(int x, int y) {
		return x + y;
	}
	namespace Nested {
		int increase(int x) {
			return x + 1;
		}
	}

	enum class Color {
		Red,
		Green,
		Blue
	};

	struct Carrier {
		enum class Translator {
			Tolc
		};
	};

	int const i = 5;
}

struct WithEnum {
	enum class Instrument {
		Guitarr,
		Flute
	};
	Instrument i = Instrument::Flute;
};

)";

  auto jsTestCode = R"(
// Nested functions are under their respective namespace
expect(m.MyNamespace.add(1, 2)).toBe(3);
expect(m.MyNamespace.Nested.increase(2)).toBe(3);

// The '_' separated versions are no longer available
try {
	m.MyNamespace_add(1, 2);
	expect(true).teBe(false)
} catch (e) {
	expect(e.message).toBe('m.MyNamespace_add is not a function');
}
try {
	m.MyNamespace_Nested_increase(2);
	expect(true).teBe(false)
} catch (e) {
	expect(e.message).toBe('m.MyNamespace_Nested_increase is not a function');
}

// Nested enums inside namespaces
const green = m.MyNamespace.Color.Green;
expect(green).toBe(m.MyNamespace.Color.Green);

// Nested enums inside namespaces inside structs
const company = m.MyNamespace.Carrier.Translator.Tolc;
expect(company).toBe(m.MyNamespace.Carrier.Translator.Tolc);

// The '_' separated versions are no longer available
expect(m.MyNamespace_Color).toBe(undefined);
expect(m.MyNamespace_Carrier).toBe(undefined);
expect(m.MyNamespace_Carrier_Translator).toBe(undefined);

// Globals within namespaces work
expect(m.MyNamespace.i).toBe(5);

// The '_' separated versions are no longer available
expect(m.MyNamespace_i).toBe(undefined);

// Ok to nest Enums within classes
const withEnum = new m.WithEnum();
expect(withEnum.i).toBe(m.WithEnum.Instrument.Flute);
withEnum.delete();

// The '_' separated versions are no longer available
expect(m.WithEnum_Instrument).toBe(undefined);
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);
}
