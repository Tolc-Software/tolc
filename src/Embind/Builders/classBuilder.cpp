#include "Embind/Builders/classBuilder.hpp"
#include "Embind/Builders/enumBuilder.hpp"
#include "Embind/Builders/functionBuilder.hpp"
#include "Embind/Builders/typeToStringBuilder.hpp"
#include "Embind/Helpers/combine.hpp"
#include "Embind/Helpers/trampolineClass.hpp"
#include "Embind/Helpers/types.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "Embind/checkType.hpp"
#include "Embind/createFunctions.hpp"
#include "Embind/getOverloadedFunctions.hpp"

#include <IR/ir.hpp>
#include <fmt/format.h>

#include <numeric>
#include <optional>
#include <set>
#include <string>
#include <tuple>
#include <vector>

namespace Embind::Builders {

namespace {

struct TrampolineFunctions {
  std::vector<Embind::Proxy::Function> virtualFunctions;
};

bool addIfVirtual(IR::Polymorphic polymorphic,
                  std::string const& baseClass,
                  Embind::Proxy::Function& emFunction,
                  TrampolineFunctions& trampoline) {
  using IR::Polymorphic;
  switch (polymorphic) {
    case Polymorphic::PureVirtual:
      trampoline.virtualFunctions.push_back(emFunction);
      emFunction.setAsPureVirtual();
      return true;
      break;
    case Polymorphic::Virtual:
      trampoline.virtualFunctions.push_back(emFunction);
      emFunction.setAsVirtual(baseClass);
      break;
    case Polymorphic::NA: break;
  }
  return false;
}
}    // namespace

std::optional<Embind::Proxy::Class>
buildClass(IR::Struct const& cppClass, Embind::Proxy::TypeInfo& typeInfo) {
  Embind::Proxy::Class jsClass(
      Embind::Helpers::removeCppTemplate(cppClass.m_name) +
          Embind::Builders::getSeparatedTypeString(cppClass.m_templateArguments,
                                                   "_"),
      cppClass.m_representation);

  TrampolineFunctions trampoline;

  auto overloadedFunctions =
      Embind::getOverloadedFunctions(cppClass.m_public.m_functions);
  // Ignore private functions
  for (auto const& function : cppClass.m_public.m_functions) {
    if (auto maybeJsFunction = buildFunction(function, typeInfo)) {
      auto jsFunction = maybeJsFunction.value();

      jsFunction.setAsClassFunction();

      if (function.m_isStatic) {
        jsFunction.setAsStatic();
      }

      if (overloadedFunctions.find(function.m_representation) !=
          overloadedFunctions.end()) {
        jsFunction.setAsOverloaded();
      }

      auto pureVirtual = addIfVirtual(function.m_polymorphic,
                                      cppClass.m_representation,
                                      jsFunction,
                                      trampoline);
      if (pureVirtual) {
        jsClass.setAsPure();
      }

      jsClass.addFunction(jsFunction);
    } else {
      return std::nullopt;
    }
  }

  for (auto const& constructor : cppClass.m_public.m_constructors) {
    if (auto maybeJsFunction = buildFunction(constructor, typeInfo)) {
      auto jsFunction = maybeJsFunction.value();

      jsFunction.setAsClassFunction();

      if (constructor.m_isStatic) {
        jsFunction.setAsStatic();
      }

      if (cppClass.m_public.m_constructors.size() > 1) {
        jsFunction.setAsOverloaded();
      }

      jsFunction.setAsConstructor();
      jsClass.addConstructor(jsFunction);
    }
  }

  for (auto const& variable : cppClass.m_public.m_memberVariables) {
    Embind::checkType(variable.m_type, typeInfo);
    std::string getter = "";
    std::optional<std::string> setter = std::nullopt;
    if (variable.m_type.m_isConst) {
      // If it is const, we can just give a reference to it
      getter = fmt::format(
          "{fullyQualifiedClassName}::{variable}",
          fmt::arg("fullyQualifiedClassName", cppClass.m_representation),
          fmt::arg("variable", variable.m_name));
    } else {
      // Not const => Need to create setters and getters
      std::tie(getter, setter) =
          Embind::createGetterSetter(jsClass.getName(),
                                     cppClass.m_representation,
                                     variable.m_name,
                                     typeInfo);
    }
    jsClass.addMemberVariable(
        variable.m_name, getter, setter, variable.m_isStatic);
  }

  // Add default constructor
  if (cppClass.m_hasImplicitDefaultConstructor) {
    auto constructor =
        Embind::Proxy::Function(jsClass.getName(), jsClass.getName());
    constructor.setAsConstructor();
    jsClass.addConstructor(constructor);
  }

  for (auto const& inherited : cppClass.m_public.m_inherited) {
    jsClass.addInherited(inherited);
  }

  for (auto const& e : cppClass.m_public.m_enums) {
    jsClass.addEnum(buildEnum(e));
  }

  if (!trampoline.virtualFunctions.empty()) {
    // There are virtual functions
    auto [name, cls] =
        Embind::Helpers::getTrampolineClass(cppClass.m_name,
                                            cppClass.m_representation,
                                            trampoline.virtualFunctions);

    typeInfo.m_trampolineClasses.insert(cls);
    jsClass.addTrampolineClass(name,
                               typeInfo.m_functionsNamespace + "::" + name);
  }

  if (typeInfo.m_classesMarkedShared.contains(cppClass.m_representation)) {
    jsClass.setAsManagedByShared();
  }

  return jsClass;
}
}    // namespace Embind::Builders
