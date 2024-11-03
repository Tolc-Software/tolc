#include "TestUtil/parse.hpp"

#include <catch2/catch_test_macros.hpp>
#include <fmt/format.h>

#include <variant>

namespace {
template <typename T>
void requireTypeIsFunction(T type) {
  auto& func = type.m_type;
  auto funcType = std::get_if<IR::Type::Function>(&func);
  REQUIRE(funcType != nullptr);
  REQUIRE(funcType->m_representation != "");
}

IR::Argument getFunctionArgument(IR::Namespace const& globalNS) {
  REQUIRE(globalNS.m_functions.size() == 1);
  auto& f = globalNS.m_functions.back();
  REQUIRE(f.m_arguments.size() == 1);
  return f.m_arguments.back();
}

IR::Type getFunctionReturnType(IR::Namespace const& globalNS) {
  REQUIRE(globalNS.m_functions.size() == 1);
  return globalNS.m_functions.back().m_returnType;
}
}    // namespace

TEST_CASE("Function returning a function", "[functionTypes]") {
  auto code = R"(
#include <functional>

std::function<void()> f();
)";
  CAPTURE(code);
  auto returnType = getFunctionReturnType(TestUtil::parseString(code));
  // Can be expanded to one of:
  //  std::function<void(void)>
  //  std::function<void (void)>
  //  std::function<void()>
  //  std::function<void ()>
  CAPTURE(returnType.m_representation);
  REQUIRE(returnType.m_representation.find("std::function<void") !=
          std::string::npos);

  requireTypeIsFunction(returnType);
}

TEST_CASE("Function taking a C-style function argument", "[functionTypes]") {
  auto code = R"(
double outerFunction(void(*argumentFunction)(int));
)";
  CAPTURE(code);
  auto arg = getFunctionArgument(TestUtil::parseString(code));
  REQUIRE(arg.m_name == "argumentFunction");
  auto type = arg.m_type;
  CAPTURE(type.m_representation);
  REQUIRE(type.m_representation.find("void") != std::string::npos);
  REQUIRE(type.m_representation.find("(int)") != std::string::npos);

  requireTypeIsFunction(type);
}

TEST_CASE("Function taking a function argument", "[functionTypes]") {
  auto code = R"(
#include <functional>

void f(std::function<void()> arg);
)";
  CAPTURE(code);
  auto arg = getFunctionArgument(TestUtil::parseString(code));
  REQUIRE(arg.m_name == "arg");
  auto type = arg.m_type;
  // Can be expanded to one of:
  //  std::function<void(void)>
  //  std::function<void (void)>
  //  std::function<void()>
  //  std::function<void ()>
  CAPTURE(type.m_representation);
  REQUIRE(type.m_representation.find("std::function<void") !=
          std::string::npos);

  requireTypeIsFunction(type);
}
