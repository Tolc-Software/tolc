#include "Embind/Builders/moduleBuilder.hpp"
#include "Embind/Builders/attributeBuilder.hpp"
#include "Embind/Builders/classBuilder.hpp"
#include "Embind/Builders/enumBuilder.hpp"
#include "Embind/Builders/functionBuilder.hpp"
#include "Embind/Helpers/combine.hpp"
#include "Embind/Helpers/split.hpp"
#include "Embind/Proxy/module.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "Embind/getOverloadedFunctions.hpp"

#include <IR/ir.hpp>
#include <fmt/format.h>

#include <algorithm>
#include <optional>
#include <set>
#include <string>

namespace Embind::Builders {

std::optional<Embind::Proxy::Module>
buildModule(IR::Namespace const& ns, Embind::Proxy::TypeInfo& typeInfo) {
  Embind::Proxy::Module builtModule(ns.m_name, ns.m_representation);

  auto overloadedFunctions = Embind::getOverloadedFunctions(ns.m_functions);
  for (auto const& function : ns.m_functions) {
    if (auto maybeF = Embind::Builders::buildFunction(function, typeInfo)) {
      auto f = maybeF.value();
      if (overloadedFunctions.find(function.m_representation) !=
          overloadedFunctions.end()) {
        f.setAsOverloaded();
      }
      builtModule.addFunction(f);
    } else {
      return std::nullopt;
    }
  }

  for (auto const& variable : ns.m_variables) {
    auto v = Embind::Builders::buildAttribute(
        ns.m_representation, variable, typeInfo);
    builtModule.addAttribute(v);
  }

  for (auto const& cls : ns.m_structs) {
    if (auto maybeC = Embind::Builders::buildClass(cls, typeInfo)) {
      auto c = maybeC.value();
      builtModule.addClass(c);
    } else {
      return std::nullopt;
    }
  }

  for (auto const& e : ns.m_enums) {
    builtModule.addEnum(Embind::Builders::buildEnum(e));
  }

  return builtModule;
}
}    // namespace Embind::Builders
