#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"
#include "TestUtil/types.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

TEST_CASE("Vectors with qualifiers", "[vector]") {
  auto code = R"(
#include <vector>

void f(std::vector<double> const& v);
void g(std::vector<double> const* w);
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto f = TestUtil::findFunction(globalNS, "f");
  REQUIRE(f.m_arguments.size() == 1);
  auto v = f.m_arguments.back();
  REQUIRE(v.m_name == "v");
  auto vec = TestUtil::getContainer(v.m_type);
  REQUIRE(vec);
  REQUIRE(vec->m_container == IR::ContainerType::Vector);
  REQUIRE(vec->m_containedTypes.size() > 1);

  auto g = TestUtil::findFunction(globalNS, "g");
  REQUIRE(g.m_arguments.size() == 1);
  auto w = g.m_arguments.back();
  REQUIRE(w.m_name == "w");
  auto wec = TestUtil::getContainer(w.m_type);
  REQUIRE(wec);
  REQUIRE(wec->m_container == IR::ContainerType::Vector);
  REQUIRE(wec->m_containedTypes.size() > 1);
}

TEST_CASE("Nested vectors", "[vector]") {
  auto code = R"(
#include <vector>

std::vector<std::vector<int>> f();
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  auto& returnType = f.m_returnType;
  auto& vecOfVecs = returnType.m_type;
  auto vectorType = std::get_if<IR::Type::Container>(&vecOfVecs);
  REQUIRE(vectorType != nullptr);
  REQUIRE(vectorType->m_container == IR::ContainerType::Vector);
  // NOTE: Also has an allocator
  REQUIRE(vectorType->m_containedTypes.size() == 2);

  auto& nestedVector = vectorType->m_containedTypes[0];
  auto nestedVectorType =
      std::get_if<IR::Type::Container>(&nestedVector.m_type);
  REQUIRE(nestedVectorType != nullptr);
  REQUIRE(nestedVectorType->m_container == IR::ContainerType::Vector);
  // NOTE: Also has an allocator
  REQUIRE(nestedVectorType->m_containedTypes.size() == 2);
  REQUIRE(nestedVectorType->m_containedTypes[0].m_representation == "int");
}

TEST_CASE("std::vector<baseType>", "[vector]") {
  for (auto baseType : TestUtil::getBaseTypes(
           /* excluding */ {"std::string", "void"})) {
    auto code = R"(
#include <vector>

struct MyClass {
)" +
                fmt::format(R"(
	std::vector<{baseType}> m_v;)",
                            fmt::arg("baseType", baseType)) +
                R"(
};

)";
    CAPTURE(code);
    auto globalNS = TestUtil::parseString(code);
    auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
    auto& m_v =
        TestUtil::findMember(myClass, "m_v", TestUtil::AccessModifier::Public);

    REQUIRE(
        m_v.m_type.m_representation ==
        fmt::format("std::vector<{baseType}>", fmt::arg("baseType", baseType)));
    auto vectorType = std::get_if<IR::Type::Container>(&m_v.m_type.m_type);
    REQUIRE(vectorType != nullptr);

    REQUIRE(vectorType->m_container == IR::ContainerType::Vector);
    // {{baseType}, std::allocator<{baseType}>}
    REQUIRE(vectorType->m_containedTypes.size() == 2);
    for (auto const& type : vectorType->m_containedTypes) {
      if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
        auto base = TestUtil::getIRFromString(baseType);
        REQUIRE(base.has_value());
        REQUIRE(value->m_base == base.value());
        REQUIRE(type.m_isConst == false);
        REQUIRE(type.m_isReference == false);
        REQUIRE(type.m_representation == baseType);
      } else if (auto allocator =
                     std::get_if<IR::Type::Container>(&type.m_type)) {
        REQUIRE(allocator->m_container == IR::ContainerType::Allocator);
        REQUIRE(type.m_isConst == false);
        REQUIRE(type.m_isReference == false);
        REQUIRE((type.m_representation ==
                     fmt::format("std::allocator<{baseType}>",
                                 fmt::arg("baseType", baseType)) ||
                 type.m_representation ==
                     fmt::format("std::__1::allocator<{baseType}>",
                                 fmt::arg("baseType", baseType))));

        REQUIRE(allocator->m_containedTypes.size() == 1);
        auto& allocInt = allocator->m_containedTypes.back();
        REQUIRE(allocInt.m_isConst == false);
        REQUIRE(allocInt.m_isReference == false);
        REQUIRE(allocInt.m_representation == baseType);

        auto valueInt = std::get_if<IR::Type::Value>(&allocInt.m_type);
        REQUIRE(valueInt != nullptr);
        auto base = TestUtil::getIRFromString(baseType);
        REQUIRE(base.has_value());
        REQUIRE(valueInt->m_base == base.value());

      } else {
        INFO(
            "The type is neither a int or allocator. Something must have gone wrong.");
        REQUIRE(false);
      }
    }
  }
}
