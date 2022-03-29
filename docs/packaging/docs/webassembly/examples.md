# Examples #

Each example is taken from the test suite for `Tolc` and, given that you use the latest version, you can expect them all to work.

To use `WebAssembly` from `javascript`, one has to load it in asynchronously. When using `Tolc` this is done with a [`Promise`](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Promise) on the `javascript` side. Each library named `MyLib` exports a `Promise` called `loadMyLib`, in every test the name is simply `m` for brevity. All tests use [`jest`](https://jestjs.io/), and the `javascript` test boiler plate is omitted:

```javascript
const loadm = require('./build/m');

test('Tolc Test', () => {
	loadm().then(m => {
		// The actual javascript example body goes here
		expect(m.sayTen()).toBe(10);
	});
});
```


## Classes ##


```cpp

#include <string>
#include <string_view>

class WithConstructor {
public:
	explicit WithConstructor(std::string s) : m_s(s) {}

	// There is a separate .cpp file containing
	// int const WithConstructor::i;
	// To initialize i
	static int const i = 5;

	std::string getS() { return m_s; }

	static int getStatic() { return 55; }

private:
	std::string m_s;
};

struct WithMembers {
	int const i = 5;
	std::string s = "hello";
};

class WithFunction {
public:
	int add(int i, int j) {
		return i + j;
	}
};

class WithPrivateFunction {
	double multiply(double i, double j) {
		return i * j;
	}
};

namespace MyNamespace {
	struct Nested {
		int const i = 42;
	};
}

struct WithEnum {
	enum class Instrument {
		Guitarr,
		Flute
	};
	Instrument i = Instrument::Flute;
};




```


```javascript

// Statics are available without instantiation
// Static function
expect(m.WithConstructor.getStatic()).toBe(55);
// Static variable
expect(m.WithConstructor.i).toBe(5);

const withConstructor = new m.WithConstructor("Hello");
expect(withConstructor.getS()).toBe("Hello");

// Classes need to be deleted manually
withConstructor.delete();

// Const properties are read-only
const withMembers = new m.WithMembers();
expect(withMembers.i).toBe(5);
try {
	withMembers.i = 10;
	expect(true).toBe(false);
} catch (err) {
	expect(err.toString()).toMatch(/BindingError: WithMembers.i is a read-only property/i);
}
expect(withMembers.s).toBe("hello");
withMembers.delete();

// Public functions are available
const withFunction = new m.WithFunction();
expect(withFunction.add(5, 10)).toBe(15);
withFunction.delete();

// Cannot access private functions
const withPrivateFunction = new m.WithPrivateFunction();
try {
	withPrivateFunction.multiply(5, 10);
	expect(true).toBe(false);
} catch (err) {
	expect(err.toString()).toMatch(/TypeError: withPrivateFunction.multiply is not a function/i);
}
withPrivateFunction.delete();

// Classes can be found under their namespace
const nested = new m.MyNamespace.Nested();
expect(nested.i).toBe(42);
nested.delete();

// Ok to nest Enums within classes
const withEnum = new m.WithEnum();
expect(withEnum.i).toBe(m.WithEnum.Instrument.Flute);
withEnum.delete();

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
	enum class Inside {
		One,
		Two
	};

	explicit EnumTest(Scoped s) : memberEnum(s), inside(Inside::One) {};

	Inside inside;

	Scoped memberEnum;
};

Unscoped echo(Unscoped s) {
	return s;
}

namespace MyNamespace {
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
}


```


```javascript

// Can be passed as arguments
const snail = m.Scoped.Snail;
const enumTest = new m.EnumTest(snail);
expect(enumTest.memberEnum).toBe(snail);

// Nested enums within classes
expect(enumTest.inside).toBe(m.EnumTest.Inside.One);
enumTest.delete();

// Unscoped enums work exactly the same
const uboat = m.Unscoped.Uboat;
expect(m.echo(uboat)).toBe(uboat);

// Nested enums inside namespaces
const green = m.MyNamespace.Color.Green;
expect(green).toBe(m.MyNamespace.Color.Green);

// Nested enums inside namespaces inside structs
const company = m.MyNamespace.Carrier.Translator.Tolc;
expect(company).toBe(m.MyNamespace.Carrier.Translator.Tolc);

```



## Functions ##


```cpp

#include <string>

int sayTen() {
	return 10;
}

std::string giveBack(std::string const& s) {
	return s;
}

namespace MyNamespace {
	int add(int x, int y) {
		return x + y;
	}
	namespace Nested {
		int increase(int x) {
			return x + 1;
		}
	}
}

```


```javascript

expect(m.sayTen()).toBe(10);

expect(m.giveBack("hello")).toBe("hello");

// Nested functions are under their respective namespace
expect(m.MyNamespace.add(1, 2)).toBe(3);
expect(m.MyNamespace.Nested.increase(2)).toBe(3);

```



## Global Variables ##


```cpp

#include <string_view>

int const i = 0;
double const d = 55;
std::string_view const stringView = "Hello world";
const char* charPtr = "Hello world";

namespace MyNamespace {
	int const i = 5;
}

```


```javascript

expect(m.i).toBe(0);
expect(m.d).toBe(55);

// Global strings of type std::string_view and const char* are converted
// Globals of type std::string has an open issue:
//   https://github.com/emscripten-core/emscripten/issues/16275
expect(m.stringView).toBe("Hello world");
expect(m.charPtr).toBe("Hello world");

// Globals within namespaces work
expect(m.MyNamespace.i).toBe(5);

```



## Namespaces ##


```cpp

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


```


```javascript

expect(m.MyLib.complexFunction()).toBe(5);

// Namespaces can be nested arbitrarily
expect(m.MyLib.We.Are.Going.Pretty.Deep.meaningOfLife()).toBe('42');

```



## Overriding virtual in javascript ##


```cpp

#include <string>

class Animal {
public:
	virtual ~Animal() { }
	virtual std::string sound(int n_times, bool grumpy) = 0;
};

class Dog : public Animal {
public:
	std::string sound(int n_times, bool grumpy) override {
		if (grumpy) {
			return "No.";
		}

		std::string result;
		for (int i = 0; i < n_times; ++i) {
			result += "woof! ";
		}
		return result;
	}
};

std::string call_sound(Animal *animal) {
	return animal->sound(3, false);
}

```


```javascript

const fido = new m.Dog();
const grumpy = true;

// Overloaded function in C++
expect(fido.sound(1, grumpy)).toBe("No.")
expect(fido.sound(1, !grumpy)).toBe("woof! ")

// Polymorphic function in C++
expect(m.call_sound(fido)).toBe("woof! woof! woof! ")
fido.delete();

// Inherit from virtual C++ classes in javascript
const Cat = m.Animal.extend("Animal", {
  // Override C++ function
  sound: function(n_times, grumpy) {
    return grumpy ? "No." : "meow! ".repeat(n_times);
  },
});

const whiskers = new Cat();

// Overloaded C++ function in javascript
expect(whiskers.sound(1, grumpy)).toBe("No.")
expect(whiskers.sound(1, !grumpy)).toBe("meow! ")

// Polymorphic function in C++ called with javascript object
// Automatic downcasting
expect(m.call_sound(whiskers)).toBe("meow! meow! meow! ")

whiskers.delete();

// Another way is to just provide what is needed
// to implement the Animal interface
const tiger = m.Animal.implement({
  // Put only the functions you want to implement here
  sound: function(n_times, grumpy) {
    return grumpy ? "No." : "roar! ".repeat(n_times);
  },
});

expect(tiger.sound(1, grumpy)).toBe("No.")
expect(tiger.sound(1, !grumpy)).toBe("roar! ")

// Automatic downcasting works the same
expect(m.call_sound(tiger)).toBe("roar! roar! roar! ")

tiger.delete();

```



## Simple inheritence ##


```cpp

#include <string>

struct Pet {
    Pet(const std::string &name) : name(name) { }
    std::string name;
};

struct Dog : public Pet {
    Dog(const std::string &name) : Pet(name) { }
    std::string bark() const { return "woof!"; }
};

```


```javascript

const fido = new m.Dog("Fido");

// Inherits public properties
expect(fido.name).toBe("Fido")

// But has its new functions
expect(fido.bark()).toBe("woof!")

```



## Smart Pointers ##


```cpp

#include <memory>

struct Example {
	int m_value = 5;
};

struct ExampleShared {
	int m_value = 10;
};

std::unique_ptr<Example> create_unique() {
	return std::make_unique<Example>();
}

std::shared_ptr<ExampleShared> create_shared() {
	return std::make_shared<ExampleShared>();
}

```


```javascript

// Note: Embind only supports *return*-values of std::unique_ptr
//       An argument of type std::unique_ptr<T> will return in an error message

// std::unique_ptr just corresponds to the value
u = m.create_unique();
expect(u.m_value).toBe(5);
u.delete();

// std::shared_ptr also just corresponds to the value
s = m.create_shared();
expect(s.m_value).toBe(10);
s.delete();

```



## std::array ##


```cpp

#include <array>
#include <string>

std::array<double, 3> getData3() {
	return {0.0, 1.0, 2.0};
}

// Multiple array types at the same time
std::array<int, 2> getData2() {
	return {0, 1};
}


```


```javascript

const data3 = m.getData3();

// It's just a normal JS array
expect(data3.length).toBe(3);

expect(data3).toStrictEqual([0, 1, 2]);

const data2 = m.getData2();
expect(data2.length).toBe(2);

expect(data2).toStrictEqual([0, 1]);

```



## std::map ##


```cpp

#include <map>
#include <string>

std::map<int, std::string> getData() {
	std::map<int, std::string> m;
	m.insert({10, "hello"});
	return m;
}


```


```javascript

const data = m.getData();

expect(data.size()).toBe(1);

expect(data.get(10)).toBe("hello");

data.set(50, "Stuff");

expect(data.size()).toBe(2);
expect(data.get(50)).toBe("Stuff");

```



## std::pair ##


```cpp

#include <string>

class MyClass {
public:
	explicit MyClass(std::pair<std::string, int> s) : m_s(s) {}

	std::pair<std::string, int> getS() { return m_s; }

private:
	std::pair<std::string, int> m_s;
};

class WithFunction {
public:
	int sum(std::pair<int, int> v) {
		return v.first + v.second;
	}
};


```


```javascript

// On the javascript side, std::pair<std::string, int> is a basic array
const myArray = ["hi", 4];
withMember = new m.MyClass(myArray);
expect(withMember.getS()).toStrictEqual(myArray);
withMember.delete();

const withFunction = new m.WithFunction()
expect(withFunction.sum([1, 2])).toBe(3)
withFunction.delete();

```



## std::tuple ##


```cpp

#include <string>
#include <tuple>

class MyClass {
public:
	explicit MyClass(std::tuple<std::string, int> _tuple) : m_tuple(_tuple) {}

	std::tuple<std::string, int> getTuple() { return m_tuple; }

	std::tuple<std::string, int> m_tuple;
};

class WithFunction {
public:
	double sum(std::tuple<int, int, float, double> t) {
		return std::get<0>(t)
			   + std::get<1>(t)
			   + std::get<2>(t)
			   + std::get<3>(t);
	}
};


```


```javascript


// Tuple converts from javascript array
const myArray = ["Hello World", 42];
const myClass = new m.MyClass(myArray);
expect(myClass.getTuple()).toStrictEqual(myArray);

// The array still need to match the underlying std::tuple structure
try {
	// m_tuple is public
	myClass.m_tuple = [1, 2, 3];
} catch (err) {
	expect(err.toString()).toMatch(/TypeError: Incorrect number of tuple elements for tuple_string_int: expected=2, actual=3/i);
}

myClass.delete();

// Can handle different Number types
const withFunction = new m.WithFunction();
expect(withFunction.sum([1, 2, 3.3, 2.0])).toBeCloseTo(8.3, 5);

withFunction.delete();

```



## std::vector ##


```cpp

#include <vector>

std::vector<int> getData() {
	return {0, 1, 2};
}


```


```javascript

const data = m.getData();

expect(data.size()).toBe(3);

for (let i = 0; i < data.size(); i++) {
    expect(data.get(i)).toBe(i);
}

data.push_back(3);

expect(data.size()).toBe(4);

expect(data.get(3)).toBe(3);

```


