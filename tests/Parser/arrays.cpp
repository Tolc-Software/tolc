#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("array segfault case from downstream", "[arrays]") {
  auto code = R"(
#include <array>
#include <string>

class WithMember {
public:
	explicit WithMember(std::array<std::string, 2> s) : m_s(s) {}

	std::array<std::string, 2> getS() { return m_s; }

private:
	std::array<std::string, 2> m_s;
};

class WithFunction {
public:
	int sum(std::array<int, 5> v) {
		int s = 0;
		for (auto i : v) {
			s += i;
		}
		return s;
	}
};
)";

  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
}

TEST_CASE("std::array of base type", "[arrays]") {
  for (auto baseType : TestUtil::getBaseTypes(
           /* excluding */ {"std::string", "void"})) {
    auto code = R"(
#include <array>

struct MyClass {
)" +
                fmt::format(R"(
	std::array<{baseType}, 5> m_v;)",
                            fmt::arg("baseType", baseType)) +
                R"(
};

)";
    CAPTURE(code);
    auto globalNS = TestUtil::parseString(code);
    auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
    auto& m_v =
        TestUtil::findMember(myClass, "m_v", TestUtil::AccessModifier::Public);

    REQUIRE(m_v.m_type.m_representation ==
            fmt::format("std::array<{baseType}, 5>",
                        fmt::arg("baseType", baseType)));
    auto arrayType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
    REQUIRE(arrayType != nullptr);

    REQUIRE(arrayType->m_container == IR::ContainerType::Array);
    // {int, 5}
    REQUIRE(arrayType->m_containedTypes.size() == 2);
    auto& type = arrayType->m_containedTypes.front();
    auto& integral = arrayType->m_containedTypes.back();
    REQUIRE(integral.m_representation == "5");

    if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
      auto base = TestUtil::getIRFromString(baseType);
      REQUIRE(base.has_value());
      REQUIRE(value->m_base == base.value());
      REQUIRE(type.m_isConst == false);
      REQUIRE(type.m_isReference == false);
      REQUIRE(type.m_representation == baseType);
    } else {
      INFO("The type is not a base type. Something must have gone wrong.");
      REQUIRE(false);
    }
  }
}
