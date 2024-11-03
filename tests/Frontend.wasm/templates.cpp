#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Specialized templates", "[templates]") {
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
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
)";

  auto jsTestCode = R"(
hi = m.getSomething("hi")
expect(hi).toBe("hi");

five = m.getSomething(5)
expect(five).toBe(5);

l = m.getSomething(["hi"])
expect(l).toBe(["hi"]);

my_class_int = m.MyClass_int()
expect(my_class_int.myFun(25)).toBe(25);

my_class_map = m.MyClass_map_char_vector_int()
expect(my_class_map.myFun({h: [1]}).toStrictEqual({h: [1]})))";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);
}
