#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("One function depends on two structs", "[dependency]") {
  auto globalNS = TestUtil::parseString(R"(
struct Data0 {};
struct Data1 {};

struct Consumer {
	Consumer(Data0 d0, Data1 d1);
};
)");
  auto const& data0 = TestUtil::findStruct(globalNS, "Data0");
  auto const& data1 = TestUtil::findStruct(globalNS, "Data1");
  auto const& consumer = TestUtil::findStruct(globalNS, "Consumer");
  REQUIRE(consumer.m_public.m_constructors.size() == 1);
  auto const& consumerConstructor = consumer.m_public.m_constructors.back();

  // Data0 comes first in the file
  // Consumer requires both Data0 and Data1 to be defined
  REQUIRE(data0.m_id < consumer.m_id);
  REQUIRE(data1.m_id < consumer.m_id);
  REQUIRE(data0.m_id < consumerConstructor.m_id);
  REQUIRE(data1.m_id < consumerConstructor.m_id);
}

TEST_CASE("Linear dependency", "[dependency]") {
  auto globalNS = TestUtil::parseString(R"(
struct Data {};

struct Consumer {
	Consumer(Data d);
};

void f(Consumer);
)");
  auto const& data = TestUtil::findStruct(globalNS, "Data");
  auto const& consumer = TestUtil::findStruct(globalNS, "Consumer");
  REQUIRE(consumer.m_public.m_constructors.size() == 1);
  auto const& f = TestUtil::findFunction(globalNS, "f");

  // f requires Consumer which requires Data
  REQUIRE(data.m_id < consumer.m_id);
  REQUIRE(consumer.m_id < f.m_id);
}

TEST_CASE("Function with struct dependency, forward declaration",
          "[dependency]") {
  auto globalNS = TestUtil::parseString(R"(
struct Data;
Data* f();

struct Data {};
)");
  auto const& data = TestUtil::findStruct(globalNS, "Data");
  auto const& f = TestUtil::findFunction(globalNS, "f");

  // f requires Data to be defined before
  // but since it comes before it needs a forward declaration
  REQUIRE(data.m_id > f.m_id);
}

TEST_CASE("Class with nested enum", "[dependency]") {
  auto globalNS = TestUtil::parseString(R"(

namespace MyLib {
	struct Data {
		enum class Inner { I0, I1 };
	};
}

MyLib::Data::Inner f();
)");
  // {MyLib::Data, MyLib::Data::Inner, f}
  auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
  auto const& data = TestUtil::findStruct(myLib, "Data");
  auto const& inner =
      TestUtil::findEnum(data, "Inner", TestUtil::AccessModifier::Public);
  auto const& f = TestUtil::findFunction(globalNS, "f");

  // f requires Inner to be defined
  // Inner requires Data to be defined before
  // => Data, Inner, f
  REQUIRE(data.m_id < inner.m_id);
  REQUIRE(inner.m_id < f.m_id);
}

TEST_CASE("Function requires namespaced struct", "[dependency]") {
  auto globalNS = TestUtil::parseString(R"(

namespace MyLib {
	struct Data {};
}

MyLib::Data f();
)");
  auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
  auto const& data = TestUtil::findStruct(myLib, "Data");

  auto const& f = TestUtil::findFunction(globalNS, "f");
  // f requires Data to be defined before
  REQUIRE(data.m_id < f.m_id);
}

TEST_CASE("Function requires namespaced enum", "[dependency]") {
  auto globalNS = TestUtil::parseString(R"(

namespace MyLib {
	enum class Data { d0, d1 };
}

MyLib::Data f();
)");
  auto& myLib = TestUtil::findNamespace(globalNS, "MyLib");
  auto const& data = TestUtil::findEnum(myLib, "Data");

  auto const& f = TestUtil::findFunction(globalNS, "f");
  // f requires Data to be defined before
  REQUIRE(data.m_id < f.m_id);
}
