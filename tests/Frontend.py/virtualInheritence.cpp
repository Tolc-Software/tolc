#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Virtual inheritence", "[virtualInheritence]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

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

	auto pythonTestCode = R"(
fido = m.Dog()
grumpy = True

# Overloaded function in C++
self.assertEqual(fido.sound(1, grumpy), "No.")
self.assertEqual(fido.sound(1, not grumpy), "woof! ")

# Polymorphic function in C++
self.assertEqual(m.call_sound(fido), "woof! woof! woof! ")

# Inherit from virtual C++ classes in python
class Cat(m.Animal):
  # Override C++ function
  def sound(self, n_times, grumpy):
    return "No." if grumpy else "meow! " * n_times

whiskers = Cat()

# Overloaded C++ function in python
self.assertEqual(whiskers.sound(1, grumpy), "No.")
self.assertEqual(whiskers.sound(1, not grumpy), "meow! ")

# Polymorphic function in C++ called with python object
self.assertEqual(m.call_sound(whiskers), "meow! meow! meow! ")
)";

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Overriding virtual in python");
}
