#include "TestStage/paths.hpp"
#include "TestUtil/pybindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Specialized templates", "[templates]") {
	std::string moduleName = "m";
	auto stage =
	    TestUtil::PybindStage(TestStage::getRootStagePath(), moduleName);

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

template class MyClass<int>;
template class MyClass<std::map<char, std::vector<int>>>;
template class MyClass<std::array<int, 3>>;
)";

	auto pythonTestCode = fmt::format(R"(
# getSomething<std::string>
hi = {moduleName}.getSomething("hi")
self.assertEqual(hi, "hi")

# getSomething<int>
five = {moduleName}.getSomething(5)
self.assertEqual(five, 5)

# getSomething<std::vector<std::string>>
l = {moduleName}.getSomething(["hi"])
self.assertEqual(l, ["hi"])

# MyClass<int>
my_class_int = {moduleName}.MyClass_int()
self.assertEqual(my_class_int.myFun(25), 25)

# MyClass<std::map<char, std::vector<int>>>
my_class_map = {moduleName}.MyClass_map_char_vector_int()
self.assertEqual(my_class_map.myFun({{'h': [1]}}), {{'h': [1]}})

# MyClass<std::array<int, 3>>
my_class_array = {moduleName}.MyClass_array_int_3()
self.assertEqual(my_class_array.myFun([1, 2, 3]), [1, 2, 3])
)",
	                                  fmt::arg("moduleName", moduleName));

	auto errorCode = stage.runPybindTest(cppCode, pythonTestCode);
	REQUIRE(errorCode == 0);

	stage.exportAsExample("Templates");
}
