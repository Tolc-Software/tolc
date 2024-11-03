#include "Embind/createFunctions.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <fmt/format.h>

#include <string>
#include <utility>

namespace {
std::string registerFunction(std::string functionName,
                             std::string const& fullFunction,
                             Embind::Proxy::TypeInfo& typeInfo) {
  // The getters may be in a namespace
  if (!typeInfo.m_functionsNamespace.empty()) {
    functionName = typeInfo.m_functionsNamespace + "::" + functionName;
  }

  // Save them add to module later
  typeInfo.m_extraFunctions.insert(fullFunction);

  return functionName;
}
}    // namespace

namespace Embind {
std::pair<std::string, std::string>
createGetterSetter(std::string const& className,
                   std::string const& fullyQualifiedClassName,
                   std::string const& variableName,
                   Embind::Proxy::TypeInfo& typeInfo) {
  std::string getter = fmt::format(R"({className}_get_{variable})",
                                   fmt::arg("variable", variableName),
                                   fmt::arg("className", className));
  std::string setter = fmt::format(R"({className}_set_{variable})",
                                   fmt::arg("variable", variableName),
                                   fmt::arg("className", className));

  std::string getterFunction = fmt::format(
      R"(
	decltype({fullyQualifiedClassName}::{variable}) {getter}({fullyQualifiedClassName} const& c_) {{
		return c_.{variable};
	}})",
      fmt::arg("fullyQualifiedClassName", fullyQualifiedClassName),
      fmt::arg("variable", variableName),
      fmt::arg("getter", getter));

  std::string setterFunction = fmt::format(
      R"(
	void {setter}({fullyQualifiedClassName}& c_, decltype({fullyQualifiedClassName}::{variable}) const& v_) {{
		c_.{variable} = v_;
	}}
	)",
      fmt::arg("fullyQualifiedClassName", fullyQualifiedClassName),
      fmt::arg("variable", variableName),
      fmt::arg("setter", setter));

  return {registerFunction(getter, getterFunction, typeInfo),
          registerFunction(setter, setterFunction, typeInfo)};
}

std::pair<std::string, std::string>
createGetterSetter(std::string const& typeString,
                   std::string const& representation,
                   std::string const& elementType,
                   int index,
                   Embind::Proxy::TypeInfo& typeInfo) {
  std::string getter = fmt::format("{}_get_{}", typeString, index);
  std::string getterFunction =
      fmt::format(R"(
auto {getter}({representation} const& t) -> decltype(std::get<{index}>(t)) {{
	return std::get<{index}>(t);
}})",
                  fmt::arg("getter", getter),
                  fmt::arg("representation", representation),
                  fmt::arg("index", index));

  std::string setter = fmt::format("{}_set_{}", typeString, index);
  std::string setterFunction =
      fmt::format(R"(
void {setter}({representation}& t, {elementType} e) {{
	std::get<{index}>(t) = e;
}})",
                  fmt::arg("setter", setter),
                  fmt::arg("representation", representation),
                  fmt::arg("elementType", elementType),
                  fmt::arg("index", index));

  return {registerFunction(getter, getterFunction, typeInfo),
          registerFunction(setter, setterFunction, typeInfo)};
}
}    // namespace Embind
