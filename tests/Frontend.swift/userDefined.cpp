#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("User defined classes", "[userDefined]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <string>

class MyClass {
public:
	explicit MyClass(std::string s) : m_s(s) {}

	std::string* getS() { return &m_s; }

private:
	std::string m_s;
};

MyClass buildMyClass(std::string const& s) {
	return MyClass(s);
}

class Owner {
public:
	explicit Owner(MyClass m) : m_myClass(m) {};

	MyClass getMyClass() const { return m_myClass; }

private:
	MyClass m_myClass;
};

struct Point2d {
	int x;
	int y;
};

Point2d getMiddle(std::pair<Point2d, Point2d> p) {
	return {(p.first.x + p.second.x) / 2, (p.first.y + p.second.y) / 2};
}
)";

  auto objcTestCode = R"(
NSString* phrase = @"Hello from Objective-C";
mMyClass* myClass = [m buildMyClass:phrase];
assert([[myClass getS] isEqualToString:phrase]);

// Passing Objective-C classes to C++ classes
mOwner* owner = [[mOwner alloc] initWithMyClass:myClass];
assert([[[owner getMyClass] getS] isEqualToString:phrase]);

// Container of user defined classes
mPoint2d* a = [[mPoint2d alloc] init];
a.x = 1;
a.y = 0;
mPoint2d* b = [[mPoint2d alloc] init];
b.x = 3;
b.y = 0;

NSArray* points = [NSArray arrayWithObjects:a, b, nil];
mPoint2d* middle = [m getMiddle:points];
assert(middle.x == 2);
assert(middle.y == 0);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Passing classes between languages");
}
