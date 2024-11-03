#include "TestStage/paths.hpp"
#include "TestUtil/objcSwiftStage.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <string>

TEST_CASE("Smart pointers of custom types work", "[smartPointers]") {
  std::string moduleName = "m";
  auto stage =
      TestUtil::ObjcSwiftStage(TestStage::getRootStagePath(), moduleName);

  auto cppCode = R"(
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
)";

  auto objcTestCode = R"(
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
)";

  [[maybe_unused]] auto swiftTestCode = R"()";

  auto errorCode = stage.runTest(cppCode, objcTestCode, "objc");
  REQUIRE(errorCode == 0);

  stage.exportAsExample("Smart Pointers");
}
