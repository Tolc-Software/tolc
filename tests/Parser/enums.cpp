#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <string>
#include <variant>

TEST_CASE("Scoped and unscoped enums", "[enums]") {
  auto code = R"(

enum class ScopedEnum {
	Thing0,
	Thing1,
	Thing2,
};

enum UnScopedEnum {
	Thing0,
	Thing1,
	Thing2,
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_enums.size() == 2);
  auto maybeScoped = TestUtil::findWithName("ScopedEnum", globalNS.m_enums);
  auto maybeUnscoped = TestUtil::findWithName("UnScopedEnum", globalNS.m_enums);
  REQUIRE(maybeScoped.has_value());
  REQUIRE(maybeUnscoped.has_value());
  REQUIRE(maybeScoped->m_isScoped == true);
  REQUIRE(maybeUnscoped->m_isScoped == false);
  for (auto const& e : globalNS.m_enums) {
    REQUIRE(e.m_name == e.m_representation);
    REQUIRE(e.m_values.size() == 3);
    for (std::string value : {"Thing0", "Thing1", "Thing2"}) {
      REQUIRE(std::find(e.m_values.begin(), e.m_values.end(), value) !=
              e.m_values.end());
    }
  }
}

TEST_CASE("Enum in structs", "[enums]") {
  auto code = R"(

class C {
	enum class MyEnum { Value };
};

struct S {
	enum class MyEnum { Value };
};

)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_structs.size() == 2);

  auto& C = TestUtil::findStruct(globalNS, "C");
  REQUIRE(C.m_private.m_enums.size() == 1);
  auto const& cMyEnum = C.m_private.m_enums.back();
  REQUIRE(cMyEnum.m_representation == "C::MyEnum");

  auto& S = TestUtil::findStruct(globalNS, "S");
  REQUIRE(S.m_public.m_enums.size() == 1);
  auto const& sMyEnum = S.m_public.m_enums.back();
  REQUIRE(sMyEnum.m_representation == "S::MyEnum");

  for (auto const& e : {cMyEnum, sMyEnum}) {
    REQUIRE(e.m_isScoped);
    REQUIRE(e.m_values.size() == 1);
    REQUIRE(e.m_values.back() == "Value");
  }
}

TEST_CASE("enum as a type in a function", "[enums]") {
  auto code = R"(

enum class E {
	V0,
};

namespace MyLib {
	enum class E2 {
		V2,
	};
}

E f(MyLib::E2 e);
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  REQUIRE(f.m_returnType.m_isConst == false);
  REQUIRE(f.m_returnType.m_isReference == false);
  REQUIRE(f.m_returnType.m_representation == "E");

  REQUIRE(f.m_arguments.size() == 1);
  REQUIRE(f.m_arguments.size() == 1);
  auto& e = f.m_arguments.back();
  REQUIRE(e.m_name == "e");
  REQUIRE(e.m_type.m_isConst == false);
  REQUIRE(e.m_type.m_isReference == false);
  REQUIRE(e.m_type.m_representation == "MyLib::E2");
}

TEST_CASE("enum as a type in a class", "[enums]") {
  auto code = R"(

enum class E {
	V0,
};

namespace MyLib {
	enum class E2 {
		V2,
	};
}

class C {
public:
	E m_e;
private:
	MyLib::E2 m_e2;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& C = TestUtil::findStruct(globalNS, "C");
  REQUIRE(C.m_public.m_memberVariables.size() == 1);
  REQUIRE(C.m_private.m_memberVariables.size() == 1);

  {
    auto const& m_e =
        TestUtil::findMember(C, "m_e", TestUtil::AccessModifier::Public);
    REQUIRE(std::get_if<IR::Type::EnumValue>(&m_e.m_type.m_type) != nullptr);
    REQUIRE(m_e.m_type.m_representation == "E");
    REQUIRE(m_e.m_type.m_isConst == false);
    REQUIRE(m_e.m_type.m_isReference == false);
  }

  {
    auto const& m_e2 =
        TestUtil::findMember(C, "m_e2", TestUtil::AccessModifier::Private);
    REQUIRE(std::get_if<IR::Type::EnumValue>(&m_e2.m_type.m_type) != nullptr);
    REQUIRE(m_e2.m_type.m_representation == "MyLib::E2");
    REQUIRE(m_e2.m_type.m_isConst == false);
    REQUIRE(m_e2.m_type.m_isReference == false);
  }
}
