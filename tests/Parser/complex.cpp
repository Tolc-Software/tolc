#include "TestUtil/compare.hpp"
#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Complex type", "[complex]") {
  auto code = R"(
#include <complex>
struct MyClass {
	std::complex<int> c0;
	std::complex<double> c1;
	std::complex<float> c2;
};
)";
  CAPTURE(code);
  auto globalNS = TestUtil::parseString(code);
  auto& myClass = TestUtil::findStruct(globalNS, "MyClass");
  for (auto c : {"c0", "c1", "c2"}) {
    auto& complex =
        TestUtil::findMember(myClass, c, TestUtil::AccessModifier::Public);
    TestUtil::compare(complex.m_type, IR::BaseType::Complex);
  }
}
