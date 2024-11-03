#include "Frontend/Wasm/frontend.hpp"
#include "TestStage/paths.hpp"
#include "TestUtil/embindStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Smart pointers of custom types work", "[smartPointers]") {
  // NOTE: Smart pointers only work with custom defined types.
  //       This makes unique_ptr<int> not computable.
  //       If this is desirable in the future,
  //       wrap the int in a lambda and copy the value
  // Ex:
  //
  //   std::unique_ptr<int> create_int() {
  //      return std::unique_ptr<int>(new int(53));
  //   }
  //
  //   m.def("create_int", []() { return *create_int().get(); });
  std::string moduleName = "defaultModule";
  auto stage = TestUtil::EmbindStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
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
)";

  auto jsTestCode = R"(
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
)";

  auto errorCode = stage.runEmbindTest(cppCode, jsTestCode, moduleName);
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Smart Pointers");
}
