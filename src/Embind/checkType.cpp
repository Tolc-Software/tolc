#include "Embind/checkType.hpp"
#include "Embind/Builders/typeToStringBuilder.hpp"
#include "Embind/Helpers/types.hpp"
#include "Embind/Proxy/typeInfo.hpp"
#include "Embind/createFunctions.hpp"

#include <IR/ir.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include <functional>
#include <optional>
#include <queue>
#include <string>
#include <vector>

namespace Embind {

std::optional<std::string> getRegisterString(IR::ContainerType container) {
  // See https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html
  switch (container) {
    case IR::ContainerType::Array:
      return "em::value_array<std::array<{}>>(\"{}\")\n";
      break;
    case IR::ContainerType::Map: return R"(em::register_map<{}>("{}"))"; break;
    case IR::ContainerType::Vector:
      return R"(em::register_vector<{}>("{}"))";
      break;
    case IR::ContainerType::Pair:
      return "em::value_array<std::pair<{}>>(\"{}\")\n";
      break;
    case IR::ContainerType::Tuple:
      return "em::value_array<std::tuple<{}>>(\"{}\")\n";
      break;

    case IR::ContainerType::Deque:
    case IR::ContainerType::ForwardList:
    case IR::ContainerType::List:
    case IR::ContainerType::MultiMap:
    case IR::ContainerType::MultiSet:
    case IR::ContainerType::Optional:
    case IR::ContainerType::PriorityQueue:
    case IR::ContainerType::Queue:
    case IR::ContainerType::Set:
    case IR::ContainerType::Stack:
    case IR::ContainerType::UnorderedMap:
    case IR::ContainerType::UnorderedMultiMap:
    case IR::ContainerType::UnorderedMultiSet:
    case IR::ContainerType::UnorderedSet:
    case IR::ContainerType::Valarray:
    case IR::ContainerType::Variant:
      spdlog::error(
          "Container type {} does not currently have a direct translation via embind. The translation might not work.",
          Embind::Helpers::toString(container));
      break;

    case IR::ContainerType::Allocator:
    case IR::ContainerType::EqualTo:
    case IR::ContainerType::Greater:
    case IR::ContainerType::Hash:
    case IR::ContainerType::Less:
    case IR::ContainerType::SharedPtr:
    case IR::ContainerType::UniquePtr: break;
  }
  return std::nullopt;
}

/**
* Adds elements statements if the container type has them
* Ex: value_array has elements based on index of the underlying std::array
*/
std::string addElementsIfNecessary(IR::Type::Container const& c,
                                   std::string const& representation,
                                   std::string const& typeString,
                                   Embind::Proxy::TypeInfo& typeInfo) {
  std::string elements;
  if (c.m_container == IR::ContainerType::Array) {
    for (int i = 0; i < std::stoi(c.m_containedTypes.back().m_representation);
         ++i) {
      // See https://emscripten.org/docs/porting/connecting_cpp_and_javascript/embind.html#value-types
      elements += fmt::format("\t\t.element(emscripten::index<{}>())\n", i);
    }
    // Remove last \n
    elements.pop_back();
  } else if (c.m_container == IR::ContainerType::Pair) {
    elements += fmt::format("\t\t.element(&{}::first)\n", representation);
    elements += fmt::format("\t\t.element(&{}::second)", representation);
  } else if (c.m_container == IR::ContainerType::Tuple) {
    int index = 0;
    for (auto const& element : c.m_containedTypes) {
      auto [getterName, setterName] =
          Embind::createGetterSetter(typeString,
                                     representation,
                                     element.m_representation,
                                     index,
                                     typeInfo);
      // Record the element
      elements += fmt::format("\t\t.element(&{getterName}, &{setterName})\n",
                              fmt::arg("getterName", getterName),
                              fmt::arg("setterName", setterName));
      index++;
    }
    // Remove last \n
    // elements.pop_back();
  }
  return elements;
}

bool noneOfIsConst(std::vector<IR::Type> const& types) {
  return std::none_of(
      types.begin(), types.end(), [](auto const& t) { return t.m_isConst; });
}

/**
* Go through type and check if it requires any extra pybind11 includes
* E.g. vector conversion requires inclusion of <pybind11/stl.h>
*/
std::optional<std::string>
extractRegisterCommands(IR::Type const& type,
                        Embind::Proxy::TypeInfo& typeInfo) {
  if (auto container = Embind::Helpers::getContainer(type);
      container && noneOfIsConst(container->m_containedTypes)) {
    if (auto s = getRegisterString(container->m_container)) {
      auto typeString = Embind::Builders::buildTypeString(type, "_");
      auto registerCmd =
          fmt::format(fmt::runtime(s.value()),
                      Embind::Builders::getTemplateParameters(type),
                      typeString);

      registerCmd += addElementsIfNecessary(
          *container, type.m_representation, typeString, typeInfo);

      return registerCmd;
    }
  }
  return std::nullopt;
}

bool isSharedPtr(IR::ContainerType c) {
  return c == IR::ContainerType::SharedPtr;
}

std::optional<std::string>
getFirstUserDefinedRepresentation(std::vector<IR::Type> const& types) {
  if (!types.empty()) {
    if (auto userDefined = Embind::Helpers::getUserDefined(types[0])) {
      return userDefined->m_representation;
    }
  }
  return std::nullopt;
}

std::optional<std::string> extractShared(IR::Type const& type) {
  if (auto container = Embind::Helpers::getContainer(type)) {
    if (isSharedPtr(container->m_container)) {
      return getFirstUserDefinedRepresentation(container->m_containedTypes);
    }
  }
  return std::nullopt;
}

void checkType(IR::Type const& type, Embind::Proxy::TypeInfo& info) {
  std::queue<std::reference_wrapper<IR::Type const>> typesToCheck;
  typesToCheck.push(type);

  while (!typesToCheck.empty()) {
    auto& current = typesToCheck.front();

    if (auto registerCmd = extractRegisterCommands(current, info)) {
      info.m_registerCommands.insert(registerCmd.value());
    }

    if (auto sharedClass = extractShared(current)) {
      info.m_classesMarkedShared.insert(sharedClass.value());
    }

    if (auto container = Embind::Helpers::getContainer(current)) {
      for (auto const& containedType : container->m_containedTypes) {
        typesToCheck.push(containedType);
      }
    }
    typesToCheck.pop();
  }
}

}    // namespace Embind
