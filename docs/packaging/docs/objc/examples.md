# Examples #

Each example is taken from the test suite for `Tolc` and, given that you use the latest version, you can expect them all to work.

Each `C++` library named `MyLib` exports their functions and objects with a prefix of `MyLib`. In every test the library name is simply `m` for brevity. The examples that follow contains a bit of `C++` code, and the respective `Objective-C` and `Swift` code using it. Each `Objective-C` example is wrapped in the following boilerplate that is removed to make the examples more readable:

```objc
#include <m.h>

int main() {
  @autoreleasepool {
    assert([m sayHello] == "Hello");
  }
}
```

And the same for `Swift`:

```swift
import m_swift

assert(m.sayHello() == "Hello")
```


## Classes ##


```cpp

class WithConstructor {
public:
  explicit WithConstructor() : m_v(10) {}
  explicit WithConstructor(int v) : m_v(v) {}

  int getV() { return m_v; }
private:
  int m_v;
};

class WithFunction {
public:
  int add(int i, int j) {
    return i + j;
  }
};

class WithStatic {
public:
  static double getPi() {
    return 3.14;
  }

  static int const answer = 42;
};

class WithMember {
public:
  explicit WithMember() : i(10), phi(1.618) {}

  int i;
  double const phi;
};

```


```objc

// Constructors are overloaded with their argument types
mWithConstructor* ten = [[mWithConstructor alloc] init];
assert([ten getV] == 10);

mWithConstructor* five = [[mWithConstructor alloc] initWithInt:5];
assert([five getV] == 5);

// Member functions are available after construction
mWithFunction* withFunction = [[mWithFunction alloc] init];
assert([withFunction add: 2 j: 5] == 7);

// Static functions can be called
// without instantiating the class
assert([mWithStatic getPi] == 3.14);
// You can access static variables
// without instantiating the class
assert([mWithStatic answer] == 42);

// Member variables
mWithMember* member = [[mWithMember alloc] init];
assert(member.i == 10);
// i is not marked const
member.i = 5;
assert(member.i == 5);

// phi is marked const
// Cannot be assigned
assert(member.phi == 1.618);

```


## Enums ##


```cpp

enum Unscoped {
	Under,
	Uboat,
};

enum class Scoped {
	Sacred,
	Snail,
};

class EnumTest {
public:
	explicit EnumTest(Scoped _s) : s(_s) {};

	Unscoped f(Unscoped u) {
		return u;
	}

	Scoped s;
};

namespace NS {
	// Documentation describing the enum
	enum class Deep {
		Double,
		Down,
	};
}


```


```objc

// C++11 enums work
mEnumTest* enumTest = [[mEnumTest alloc] initWithScoped:mScopedSnail];
mScoped snail = mScopedSnail;
assert(enumTest.s == snail);

// Aswell as legacy enums
mUnscoped uboat = mUnscopedUboat;
assert([enumTest f:uboat] == uboat);

// Enums under namespaces are available
// under the corresponding submodule
/* deep = m.NS.Deep.Down */
/* assert(deep != m.NS.Deep.Double) */

// Documentation carries over from C++
// self.assertIn("Documentation describing the enum", m.NS.Deep.__doc__)

```


## Functions ##


```cpp

#include <filesystem>
#include <string>

int meaningOfLife() {
	return 42;
}

std::string sayHello(std::string const& name) {
	return "Hello " + name;
}

std::filesystem::path getPath() {
	return std::filesystem::path("/path/to/stuff.hpp");
}

namespace Inner {
	double pi() {
		return 3.14;
	}
}

```


```objc

// Global functions gets added to
// a purely static class with
// the name of the library
assert([m meaningOfLife] == 42);

// Strings can be used
assert([[m sayHello:@"Tolc"] isEqualToString:@"Hello Tolc"]);

// Aswell as filesystem paths
assert([[m getPath] isEqualToString:@"/path/to/stuff.hpp"]);

// Functions within namespaces
// are available with the
// namespaces names merged
assert([mInner pi] == 3.14);

```


## Global Variables ##


```cpp

#include <string>
#include <string_view>

static int i = 0;
namespace Nested {
	int life = 42;
	std::string s = "Hello World";
	constexpr std::string_view constant = "A constant";
}

```


```objc

// Starts at 0 and can be changed
assert(m.i == 0);
m.i = 5;
assert(m.i == 5);

// Nested with the same name
assert(mNested.life == 42);

// Strings also work
assert([mNested.s isEqualToString:@"Hello World"]);

// And string_view
assert([mNested.constant isEqualToString:@"A constant"]);

```


## Member Variables ##


```cpp

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

```


```objc

// Mutable member variables can be changed
mSimpleMember* simpleMember = [[mSimpleMember alloc] init];
assert([simpleMember.myString isEqualToString:@"Hello"]);
simpleMember.myString = @"Changed now!";
assert([simpleMember.myString isEqualToString:@"Changed now!"]);

mConstMember* constMember = [[mConstMember alloc] init];
assert(constMember.i == 42);

mMyLibNested* nested = [[mMyLibNested alloc] init];
assert(nested.d == 4.3);

```


## Namespaces ##


```cpp

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


```


```objc

// Namespaces corresponds to classes
// with {library name} + join(namespaces)
// where functions are static class functions
assert([mMyLib complexFunction] == 5);

// You can nest namespaces arbitrarily deep
NSString* lifeProTips = [mMyLibWeAreGoingPrettyDeep meaningOfLife];
assert([lifeProTips isEqualToString:@"42"]);

```


## Overloaded Functions ##


```cpp

#include <string>

// Overloaded free functions
std::string sayHello() {
	return "Hello!";
}

std::string sayHello(std::string to) {
	return std::string("Hello ") + to;
}

std::string sayHello(size_t times) {
	std::string greeting = "";
	for (size_t i = 0; i < times; ++i) {
		greeting += "Hello!";
	}
	return greeting;
}

class Overload {
public:
	// Overloaded constructor
	Overload() : m_s() {};
	Overload(std::string s) : m_s(s) {};

	// Overloaded class functions
	std::string getStuff() { return "Stuff"; }
	std::string getStuff(std::string customStuff) { return customStuff; }

private:
	std::string m_s;
};

```


```objc

// Overloaded functions work the same as in C++
// Free function overload
assert([[m sayHello] isEqualToString:@"Hello!"]);
assert([[m sayHelloString:@"Tolc"] isEqualToString:@"Hello Tolc"]);
assert([[m sayHelloUnsignedLongInt:2] isEqualToString:@"Hello!Hello!"]);

// Class constructor overload
mOverload* overload = [[mOverload alloc] init];
mOverload* overloadWithString = [[mOverload alloc] initWithString:@"Overloaded!"];

// Class function overload
assert([[overload getStuff] isEqualToString:@"Stuff"]);
assert([[overload getStuffString:@"Other"] isEqualToString:@"Other"]);

```


## Passing classes between languages ##


```cpp

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

```


```objc

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

```


## Smart Pointers ##


```cpp

#include <memory>

struct Data {
  int i = 5;
};

struct SharedData {
  int i = 10;
};

std::unique_ptr<Data> createData() {
  return std::make_unique<Data>();
}

// This moves the data,
// destroying it at the end
// Same as C++
int consumeData(std::unique_ptr<Data> data) {
  return data->i + 20;
}

std::shared_ptr<SharedData> createSharedData() {
  return std::make_shared<SharedData>();
}

// Does not move the data
// The pointer is valid after the function call
int consumeSharedData(std::shared_ptr<SharedData> data) {
  return data->i + 20;
}

```


```objc

// std::unique_ptr acts as a normal value
mData* data = [m createData];
assert(data.i == 5);

// This moves the data,
// destroying it at the end
// Same as C++
assert([m consumeData:data] == 25);

// Any access now results
// in undefined behaviour
// (possibly a crash)
// NSLog(@"%i", data.i);

// std::shared_ptr acts as a normal value
// But all mSharedData have their internal
// classes handled by a std::shared_ptr
mSharedData* sharedData = [m createSharedData];
assert(sharedData.i == 10);

// This copies the smart pointer,
// incrementing its counter.
// Valid to use sharedData after this call.
assert([m consumeSharedData:sharedData] == 30);

// No crash
NSLog(@"%i", sharedData.i);

```


## Templates ##


```cpp

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

```


```objc

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

```


## std::array ##


```cpp

#include <algorithm>
#include <array>

std::array<int, 3> const f() {
  return {0, 1, 2};
}

bool allOf(std::array<bool, 3> const& conditions) {
  return std::all_of(
      conditions.begin(), conditions.end(),
      [](auto c) { return c; });
}

double sum(std::array<double, 3> const& numbers) {
  double sum = 0;
  for (double number : numbers) {
    sum += number;
  }
  return sum;
}


```


```objc

// std::array corresponds to NSArray
NSArray* v = [m f];
assert([v count] == 3);

// The array contains {0, 1, 2}
assert([[v objectAtIndex:0] intValue] == 0);
assert([[v objectAtIndex:1] intValue] == 1);
assert([[v objectAtIndex:2] intValue] == 2);

// Sending NSArray into function works as well
NSArray* conditions = @[@(YES), @(YES), @(NO)];
assert([m allOf:conditions] == NO);

NSArray<NSNumber*>* toSum = @[@(1.1), @(2.2), @(3.3)];
assert([m sum:toSum] == 6.6);

// Error handling
@try {
  // Array with the wrong size
  NSArray<NSNumber*>* toSum = @[@(1.1), @(2.2)];
  // Expected size == 3
  [m sum:toSum];
  // Should throw exception before
  assert(NO);
} @catch(NSException* error) {
  assert([[error name] isEqualToString:@"TypeException"]);
  NSString* reason =
    @"The size of the array does not match the expected fixed size. Expected: 3, Got: 2.";
  assert([[error reason] isEqualToString:reason]);
}

```


## std::deque ##


```cpp

#include <string>
#include <deque>

std::deque<std::string>
surround(std::deque<std::string> d,
         std::string const& message) {
  d.push_front(message);
  d.push_back(message);
  return d;
}

```


```objc

// std::deque corresponds to NSArray
NSArray* myDeque = @[@"middle"];
NSArray* surroundedDeque =
  [m surround:myDeque message:@"surrounded"];
assert([surroundedDeque count] == 3);

assert([[surroundedDeque objectAtIndex:0]
  isEqualToString:@"surrounded"]);

assert([[surroundedDeque objectAtIndex:1]
  isEqualToString:@"middle"]);

assert([[surroundedDeque objectAtIndex:2]
  isEqualToString:@"surrounded"]);

```


## std::filesystem::path ##


```cpp

#include <filesystem>
#include <vector>

std::filesystem::path
takingPath(std::filesystem::path const& p) {
	return p;
}

std::filesystem::path
parent(std::filesystem::path const& p) {
	return p.parent_path();
}

std::filesystem::path
joinPaths(std::vector<std::filesystem::path> arrayToSum) {
	std::filesystem::path sum;
	for (auto f : arrayToSum) {
		sum /= f;
	}
	return sum;
}

```


```objc

// std::filesystem::path corresponds to NSString
NSString* path = @"Hello/my/name/is/Tolc";

// Passing through a function
NSString* result = [m takingPath:path];
assert([result isEqualToString:path]);

NSString* parent = [m parent:path];
assert([parent isEqualToString:@"Hello/my/name/is"]);

NSArray* paths = @[@"to", @"the", @"heart"];
NSString* joined = [m joinPaths:paths];
assert([joined isEqualToString:@"to/the/heart"]);

```


## std::list ##


```cpp

#include <string>
#include <list>

std::list<std::string> getList() {
  return {"Linked", "list", "fun"};
}

```


```objc

// std::list corresponds to NSArray
NSArray* words = [m getList];
assert([words count] == 3);

assert([[words objectAtIndex:0] isEqualToString:@"Linked"]);
assert([[words objectAtIndex:1] isEqualToString:@"list"]);
assert([[words objectAtIndex:2] isEqualToString:@"fun"]);

```


## std::map ##


```cpp

#include <map>
#include <string>
#include <vector>

std::map<std::string, int> getThings() {
  return {{"Greetings", 5}};
}

std::map<std::string, std::vector<double>> getCities() {
  return {
  {"Stockholm",
    {59.33, 18.06}},
  {"San Francisco",
    {37.77, -122.43}}
  };
}

```


```objc

// std::map translates to a NSDictionary
NSDictionary* dict = [m getThings];
assert([dict count] == 1);
NSNumber* n = [dict objectForKey:@"Greetings"];
assert(n != nil);
assert([n intValue] == 5);

// Nested containers work as well
NSDictionary* cities = [m getCities];
assert([cities count] == 2);
NSArray* stockholm = [cities objectForKey:@"Stockholm"];
assert(stockholm != nil);
assert([stockholm count] == 2);
assert([[stockholm objectAtIndex:0] doubleValue] == 59.33);
assert([[stockholm objectAtIndex:1] doubleValue] == 18.06);

NSArray* sanFrancisco = [cities objectForKey:@"San Francisco"];
assert(sanFrancisco != nil);
assert([sanFrancisco count] == 2);
assert([[sanFrancisco objectAtIndex:0] doubleValue] == 37.77);
assert([[sanFrancisco objectAtIndex:1] doubleValue] == -122.43);

```


## std::optional ##


```cpp

#include <optional>
#include <string>

std::string
answer(std::optional<std::string> const& question) {
  if (question) {
    return "Please be more specific.";
  }
  return "That's no question!";
}


```


```objc

// std::optional is either the value or nil
NSString* answer = [m answer:@"How do I take over the world?"];
assert([answer isEqualToString:@"Please be more specific."]);

// nil is the equivalent of std::nullopt on the C++ side
NSString* noAnswer = [m answer:nil];
assert([noAnswer isEqualToString:@"That's no question!"]);

```


## std::pair ##


```cpp

#include <string>

class Greeter {
public:
  explicit Greeter(std::pair<std::string, int> greetings)
    : m_greetings(greetings) {}

  std::pair<std::string, int> getGreetings() {
    return m_greetings;
  }

  std::string joinGreetings() {
    std::string joined;
    for (int i = 0; i < m_greetings.second; ++i) {
      joined += m_greetings.first;
    }
    return joined;
  }

private:
  std::pair<std::string, int> m_greetings;
};

```


```objc

// std::pair corresponds to a NSArray
// with two values
NSArray* greetings = [NSArray
  arrayWithObjects:@"Hey ", @(3), nil];
assert([greetings count] == 2);

// Sending a pair to a function
mGreeter* g = [[mGreeter alloc]
  initWithPairStringInt:greetings];

// Joining the greetings 3 times
NSString* joined = [g joinGreetings];
assert([joined isEqualToString:@"Hey Hey Hey "]);

// Error handling
@try {
  // Sending an array with size != 2
  NSArray* tooManyArgs =
    [greetings arrayByAddingObject:@"Oh no"];
  mGreeter* boom = [[mGreeter alloc]
    initWithPairStringInt:tooManyArgs];
  // Should throw exception before
  assert(NO);
} @catch(NSException* error) {
  assert([[error name] isEqualToString:@"TypeException"]);
  NSString* reason =
    @"The array passed does not match the number of types in a pair. Expected: 2, Got: 3.";
  assert([[error reason] isEqualToString:reason]);
}

```


## std::set ##


```cpp

#include <set>
#include <string>

std::set<std::string> getLanguages() {
	return {"English", "Spanish"};
}

```


```objc

// std::set corresponds to NSOrderedSet
NSOrderedSet* languages = [m getLanguages];
assert([languages count] == 2);
assert([languages containsObject:@"English"]);
assert([languages containsObject:@"Spanish"]);

```


## std::tuple ##


```cpp

#include <string>
#include <tuple>

std::tuple<int, std::string> sumInts(std::tuple<int, int, std::string> t) {
  // Sum the first two elements
  return {
    std::get<0>(t) + std::get<1>(t),
    std::get<2>(t)
  };
}

```


```objc

// std::tuple corresponds to a NSArray
// with the same amount of values
NSArray* toSum = [NSArray
  arrayWithObjects:@(1), @(2), @"Hello", nil];
assert([toSum count] == 3);

// Sending a tuple to a function
NSArray* summed = [m sumInts:toSum];
assert([summed count] == 2);
assert([[summed objectAtIndex:0] intValue] == 3);
assert([[summed objectAtIndex:1] isEqualToString:@"Hello"]);

// Error handling
@try {
  // Sending an array with size != 3
  NSArray* tooManyArgs =
    [toSum arrayByAddingObject:@"Boom"];
  [m sumInts:tooManyArgs];
  // Should throw exception before
  assert(NO);
} @catch(NSException* error) {
  assert([[error name] isEqualToString:@"TypeException"]);
  NSString* reason =
    @"The array passed does not match the number of types expected in the tuple. Expected: 3, Got: 4.";
  assert([[error reason] isEqualToString:reason]);
}

```


## std::unordered_map ##


```cpp

#include <string>
#include <unordered_map>

std::unordered_map<std::string, int>
getUnordered() {
  return {{"Unordered", 1}};
}

```


```objc

// std::unordered_map translates to a NSDictionary
NSDictionary* dict = [m getUnordered];
assert([dict count] == 1);
NSNumber* n = [dict objectForKey:@"Unordered"];
assert(n != nil);
assert([n intValue] == 1);

```


## std::unordered_set ##


```cpp

#include <string>
#include <unordered_set>

std::unordered_set<std::string> getLanguages() {
	return {"C++", "Objective-C"};
}

```


```objc

// std::unordered_set corresponds to NSSet
NSSet* languages = [m getLanguages];
assert([languages count] == 2);
assert([languages containsObject:@"C++"]);
assert([languages containsObject:@"Objective-C"]);

```


## std::valarray ##


```cpp

#include <valarray>

std::valarray<int> getIt() {
	return {0, 1, 2};
}

```


```objc

// std::valarray corresponds to NSArray
NSArray* v = [m getIt];
assert([v count] == 3);

// The vector contains {0, 1, 2}
assert([[v objectAtIndex:0] intValue] == 0);
assert([[v objectAtIndex:1] intValue] == 1);
assert([[v objectAtIndex:2] intValue] == 2);

```


## std::vector ##


```cpp

#include <algorithm>
#include <vector>

std::vector<int> f() {
  return {0, 1, 2};
}

bool allOf(std::vector<bool> const& conditions) {
  return std::all_of(
      conditions.begin(), conditions.end(),
      [](auto c) { return c; });
}

double sum(std::vector<double> const& numbers) {
  double sum = 0;
  for (double number : numbers) {
    sum += number;
  }
  return sum;
}


```


```objc

// std::vector corresponds to NSArray
NSArray* v = [m f];
assert([v count] == 3);

// The vector contains {0, 1, 2}
assert([[v objectAtIndex:0] intValue] == 0);
assert([[v objectAtIndex:1] intValue] == 1);
assert([[v objectAtIndex:2] intValue] == 2);

// Sending NSArray into function works as well
NSArray* conditions = @[@(YES), @(YES), @(NO)];
assert([m allOf:conditions] == NO);

NSArray<NSNumber*>* toSum = @[@(1.1), @(2.2), @(3.3)];
assert([m sum:toSum] == 6.6);

```

