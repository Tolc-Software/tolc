#include "TestUtil/finders.hpp"
#include "TestUtil/parse.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <vector>

void requireContainsOperators(IR::Namespace& ns,
                              std::vector<IR::Operator> const& operators) {
  auto const& opStruct = TestUtil::findStruct(ns, "Operator");
  auto const& publicOperators = opStruct.m_public.m_operators;
  REQUIRE(publicOperators.size() == operators.size());
  for (auto const& op : operators) {
    auto it = std::find_if(
        publicOperators.begin(),
        publicOperators.end(),
        [op](auto const& opAndFunction) { return opAndFunction.first == op; });
    REQUIRE(it != publicOperators.end());
  }
}

TEST_CASE("Operators gets parsed", "[operators]") {
  auto globalNS = TestUtil::parseString(R"(
struct Operator {
  // +-*/&
  Operator operator+(int);
  Operator operator-(int);
  Operator operator*(int);
  Operator operator/(int);
  Operator operator%(int);

  // Assignment
  Operator& operator=(Operator&& other);
  Operator& operator+=(Operator&& other);
  Operator& operator-=(Operator&& other);
  Operator& operator*=(Operator&& other);
  Operator& operator/=(Operator&& other);
  Operator& operator%=(Operator&& other);

  // {in,de}crement
  Operator& operator++();
  Operator& operator--();

  // Shift
  Operator operator<<(const Operator& other);
  Operator operator>>(const Operator& other);

  // Comparisons
  bool operator==(const Operator& rhs);
  bool operator!=(const Operator& rhs);
  bool operator< (const Operator& rhs);
  bool operator> (const Operator& rhs);
  bool operator<=(const Operator& rhs);
  bool operator>=(const Operator& rhs);

  // Subscript
  Operator& operator[](unsigned idx);

  // Call
  double operator()(double x);
};
)");
  using IR::Operator;
  requireContainsOperators(globalNS,
                           {Operator::AddEqual,
                            Operator::Assignment,
                            Operator::Call,
                            Operator::Decrement,
                            Operator::DivEqual,
                            Operator::Division,
                            Operator::Equal,
                            Operator::GreaterThan,
                            Operator::GreaterThanOrEqualTo,
                            Operator::Increment,
                            Operator::LeftShift,
                            Operator::LessThan,
                            Operator::LessThanOrEqualTo,
                            Operator::ModEqual,
                            Operator::Modulus,
                            Operator::MulEqual,
                            Operator::Multiplication,
                            Operator::NotEqual,
                            Operator::RightShift,
                            Operator::SubEqual,
                            Operator::Subscript,
                            Operator::Subtraction,
                            Operator::Addition});
}
