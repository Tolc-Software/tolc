#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"
#include "TestUtil/files.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Virtual inheritence", "[virtualInheritence]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
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
)";

  auto jsTestCode = R"(
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
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Overriding virtual in javascript");
}
