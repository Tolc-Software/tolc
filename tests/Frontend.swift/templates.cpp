#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Specialized templates", "[templates]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
#include <array>
#include <map>
#include <string>
#include <vector>

template <typename T>
T getSomething(T something) {
  return something;
}

template std::string getSomething(std::string something);
template int getSomething(int);
template std::vector<std::string> getSomething(std::vector<std::string>);

template <typename T>
class MyClass {
public:
T myFun(T type) {
	return type;
}
};

MyClass<char> getMyClass(MyClass<char> c) {
	return c;
}

template class MyClass<int>;
template class MyClass<std::array<int, 3>>;
)";

  auto objcTestCode = R"(
// getSomething<std::string>
NSString* hi = [m getSomethingString:@"Hi"];
assert([hi isEqualToString:@"Hi"]);

// getSomething<int>
int five = [m getSomethingInt:5];
assert(five == 5);

// getSomething<std::vector<std::string>>
NSArray* v = [m getSomethingVectorString:@[@"Hi"]];
assert([v count] == 1);
assert([[v objectAtIndex:0] isEqualToString:@"Hi"]);

// MyClass<char>
mMyClassChar* myClassChar = [[mMyClassChar alloc] init];;
assert([myClassChar myFun:25] == 25);;
// Still the same after passing through a function
mMyClassChar* passedThrough = [m getMyClass:myClassChar];;
assert([passedThrough myFun:25] == 25);;

// MyClass<int>
mMyClassInt* myClassInt = [[mMyClassInt alloc] init];
assert([myClassInt myFun:25] == 25);

// MyClass<std::array<int, 3>>
mMyClassArrayInt3* myClassArray = [[mMyClassArrayInt3 alloc] init];
NSArray* arr = [myClassArray myFun:@[@(0), @(1), @(2)]];
assert([arr count] == 3);
assert([[arr objectAtIndex:0] intValue] == 0);
assert([[arr objectAtIndex:1] intValue] == 1);
assert([[arr objectAtIndex:2] intValue] == 2);
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Templates");
}
