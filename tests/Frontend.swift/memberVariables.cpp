#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Classes", "[namespaces]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

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

  auto objcTestCode = R"(
// Mutable member variables can be changed
mSimpleMember* simpleMember = [[mSimpleMember alloc] init];
assert([simpleMember.myString isEqualToString:@"Hello"]);
simpleMember.myString = @"Changed now!";
assert([simpleMember.myString isEqualToString:@"Changed now!"]);

mConstMember* constMember = [[mConstMember alloc] init];
assert(constMember.i == 42);

mMyLibNested* nested = [[mMyLibNested alloc] init];
assert(nested.d == 4.3);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Member Variables");
}
