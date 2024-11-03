#include "TestStage/paths.hpp"
#include "TestUtil/files.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Operators", "[operators]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

	auto cppCode = R"(
#include <string>

class MyClass {
public:
  explicit MyClass(int v) : value(v) {}

  // +-*/&
  MyClass operator+(int i) { return MyClass(value + i); }
  MyClass operator-(int i) { return MyClass(value - i); }
  MyClass operator*(int i) { return MyClass(value * i); }
  MyClass operator/(int i) { return MyClass(value / i); }
  MyClass operator%(int i) { return MyClass(value % i); }

  // Assignment
  MyClass& operator+=(const MyClass& rhs) { value += rhs.value; return *this; }
  MyClass& operator-=(const MyClass& rhs) { value -= rhs.value; return *this; }
  MyClass& operator*=(const MyClass& rhs) { value *= rhs.value; return *this; }
  MyClass& operator/=(const MyClass& rhs) { value /= rhs.value; return *this; }
  MyClass& operator%=(const MyClass& rhs) { value %= rhs.value; return *this; }

  // Comparisons
  bool operator==(const MyClass &rhs) { return value == rhs.value; }
  bool operator!=(const MyClass &rhs) { return value != rhs.value; }
  bool operator<(const MyClass &rhs) { return value < rhs.value; }
  bool operator>(const MyClass &rhs) { return value > rhs.value; }
  bool operator<=(const MyClass &rhs) { return value <= rhs.value; }
  bool operator>=(const MyClass &rhs) { return value >= rhs.value; }

  // Subscript
  MyClass operator[](unsigned idx) { return MyClass(static_cast<int>(idx)); }

  // Call
  int operator()(int x) { return value + x; }
  std::string operator()(std::string const& x) { return x + std::to_string(value); }

  int value;
};
)";

	auto pythonTestCode = R"(
my_class = m.MyClass(10)
self.assertEqual(my_class.value, 10)

# Normal operators translate as expected
self.assertEqual((my_class + 5).value, 15)
self.assertEqual((my_class - 5).value, 5)
self.assertEqual((my_class * 5).value, 50)
self.assertEqual((my_class / 5).value, 2)
self.assertEqual((my_class % 3).value, 1)

other = m.MyClass(5)
# Comparison operators
self.assertTrue(my_class != other)
self.assertTrue(my_class > other)
self.assertTrue(my_class >= other)

self.assertFalse(my_class == other)
self.assertFalse(my_class < other)
self.assertFalse(my_class <= other)

# Can also use the {operator}= functions
# other.value = 5
my_class += other
self.assertEqual(my_class.value, 15)
my_class -= other
self.assertEqual(my_class.value, 10)
my_class *= other
self.assertEqual(my_class.value, 50)
my_class /= other
self.assertEqual(my_class.value, 10)
my_class %= other
self.assertEqual(my_class.value, 0)

# Subscript []
self.assertEqual(my_class[100].value, 100)

# Call ()
self.assertEqual(my_class(100), 100)
# Overloading works
self.assertEqual(my_class("The inner value is: "), "The inner value is: 0")
)";

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Operators");
}
