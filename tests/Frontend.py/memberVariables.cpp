#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Classes", "[namespaces]") {
  std::string moduleName = "m";
  auto stage = TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

class SimpleMember {
public:
	explicit SimpleMember() : myString("Hello") {}

	std::string myString;
};

class ConstMember {
public:
	const int i = 42;
};

class PrivateMember {
public:
	explicit PrivateMember(std::string s) : myString(s) {}

private:
	std::string myString;
};

namespace MyLib {

	class Nested {
	public:
		double d = 4.3;
	};
}

)";

  auto pythonTestCode = fmt::format(R"(
# Mutable member variables can be changed
simpleMember = {moduleName}.SimpleMember()
self.assertEqual(simpleMember.myString, "Hello")
simpleMember.myString = "Changed now!"
self.assertEqual(simpleMember.myString, "Changed now!")

constMember = {moduleName}.ConstMember()
self.assertEqual(constMember.i, 42)

# Const member variables cannot be changed
with self.assertRaises(AttributeError) as error_context:
    constMember.i = 0

self.assertEqual(len(error_context.exception.args), 1)
self.assertTrue(
    "'ConstMember' object has no setter" in error_context.exception.args[0]
)

# Private member variables are not available
with self.assertRaises(AttributeError) as error_context:
    privateMember = {moduleName}.PrivateMember("Hello")
    print(privateMember.myString)

self.assertEqual(len(error_context.exception.args), 1)
self.assertEqual(
    "'{moduleName}.PrivateMember' object has no attribute 'myString'",
    error_context.exception.args[0],
    "Prohibiting changing const variables does not work!",
)

nested = {moduleName}.MyLib.Nested()
self.assertEqual(nested.d, 4.3)
)",
                                    fmt::arg("moduleName", moduleName));

  auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Member Variables");
}
