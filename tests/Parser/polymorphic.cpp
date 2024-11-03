#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Static and const member variables", "[classes]") {
  auto globalNS = TestUtil::parseString(R"(
class Polymorphic {
public:
	virtual int pureVirtual() = 0;

	virtual int withDefault() { return 5; };

	int normal();
};
)");
  auto& polymorphic = TestUtil::findStruct(globalNS, "Polymorphic");
  auto const& pureVirtual = TestUtil::findFunction(
      polymorphic, "pureVirtual", TestUtil::AccessModifier::Public);
  REQUIRE(pureVirtual.m_polymorphic == IR::Polymorphic::PureVirtual);

  auto const& withDefault = TestUtil::findFunction(
      polymorphic, "withDefault", TestUtil::AccessModifier::Public);
  REQUIRE(withDefault.m_polymorphic == IR::Polymorphic::Virtual);

  auto const& normal = TestUtil::findFunction(
      polymorphic, "normal", TestUtil::AccessModifier::Public);
  REQUIRE(normal.m_polymorphic == IR::Polymorphic::NA);
}
