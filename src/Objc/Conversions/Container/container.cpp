#include "Objc/Conversions/Container/container.hpp"
#include "Objc/Conversions/Container/helpers.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"

#include <IR/ir.hpp>
#include <fmt/format.h>

#include <functional>
#include <queue>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace Objc::Conversions::Container {

std::string addArrayValue(IR::ContainerType arrayType,
                          std::string const& conversionFunction) {
  using IR::ContainerType;
  switch (arrayType) {
    // Fixed size containers
    case ContainerType::Valarray:
    case ContainerType::Array: {
      return fmt::format("cppArray[i] = {}([v objectAtIndex:i]);",
                         conversionFunction);
    }
    default: break;
  }
  return fmt::format("cppArray.push_back({}([v objectAtIndex:i]));",
                     conversionFunction);
}

std::pair<std::string, std::string>
getArrayTraversal(IR::ContainerType arrayType) {
  using IR::ContainerType;
  switch (arrayType) {
    // Need to iterate through list
    case ContainerType::List: {
      return {"value", "auto const& value : v"};
    }
    default: break;
  }
  return {"v[i]", "size_t i = 0; i < v.size(); i++"};
}

void convertArrayType(ContainerData data) {
  // Vector should have one contained value (plus allocators)
  auto containedConversions = Objc::Conversions::getConversionContainerName(
      data.containedTypes.front());

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}(NSArray* v) {{
{errorCheck}
  {noQualifiers} cppArray{init};
  for (size_t i = 0; i < [v count]; i++) {{
    {addArrayValue}
  }}
  return cppArray;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("errorCheck", getErrorCheck(data)),
      fmt::arg("init",
               data.containerType == IR::ContainerType::Valarray ?
                   "([v count])" :
                   ""),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg(
          "addArrayValue",
          addArrayValue(data.containerType, containedConversions.m_toCpp))));

  auto [value, traversal] = getArrayTraversal(data.containerType);
  data.functions.push_back(fmt::format(
      R"(
NSArray* {toObjcName}({noQualifiers} const& v) {{
  NSMutableArray* objcArray = [NSMutableArray arrayWithCapacity:v.size()];
  for ({traversal}) {{
    [objcArray addObject: {convertToObjc}({value})];
  }}
  return objcArray;
}})",
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("traversal", traversal),
      fmt::arg("value", value),
      fmt::arg("convertToObjc", containedConversions.m_toObjc)));
}

void convertMapType(ContainerData data) {
  // map should have two contained value (plus allocators)
  // TODO: Handle error
  auto keyConversion =
      Objc::Conversions::getConversionContainerName(data.containedTypes[0]);
  auto valueConversion =
      Objc::Conversions::getConversionContainerName(data.containedTypes[1]);

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}(NSDictionary* m) {{
  {noQualifiers} cppMap;
  for (id key in m) {{
    id value = [m objectForKey:key];
    cppMap[{keyConversion}(key)] = {valueConversion}(value);
  }}
  return cppMap;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("keyConversion", keyConversion.m_toCpp),
      fmt::arg("valueConversion", valueConversion.m_toCpp)));

  data.functions.push_back(fmt::format(
      R"(
NSDictionary* {toObjcName}({noQualifiers} const& m) {{
  NSMutableDictionary* objcMap = [NSMutableDictionary dictionaryWithCapacity:m.size()];
  for (auto const& keyValue : m) {{
    objcMap[{keyConversion}(keyValue.first)] = {valueConversion}(keyValue.second);
  }}
  return objcMap;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("keyConversion", keyConversion.m_toObjc),
      fmt::arg("valueConversion", valueConversion.m_toObjc)));
}

void convertSetType(ContainerData data, bool isOrdered) {
  auto valueConversion =
      Objc::Conversions::getConversionContainerName(data.containedTypes[0]);

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}(NS{ordered}Set* s) {{
  {noQualifiers} cppSet;
  for (id value in s) {{
    cppSet.insert({valueConversion}(value));
  }}
  return cppSet;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("ordered", isOrdered ? "Ordered" : ""),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("valueConversion", valueConversion.m_toCpp)));

  data.functions.push_back(fmt::format(
      R"(
NS{ordered}Set* {toObjcName}({noQualifiers} const& s) {{
  NSMutable{ordered}Set* objcSet = [NSMutable{ordered}Set {initOrdered}WithCapacity:s.size()];
  for (auto const& value : s) {{
    [objcSet addObject:{valueConversion}(value)];
  }}
  return objcSet;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("ordered", isOrdered ? "Ordered" : ""),
      fmt::arg("initOrdered", isOrdered ? "orderedSet" : "set"),
      fmt::arg("valueConversion", valueConversion.m_toObjc)));
}

void convertPairType(ContainerData data) {
  auto firstConversion =
      Objc::Conversions::getConversionContainerName(data.containedTypes[0]);
  auto secondConversion =
      Objc::Conversions::getConversionContainerName(data.containedTypes[1]);

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}(NSArray* p) {{
{errorCheck}
  {noQualifiers} cppPair;
  cppPair.first = {firstConversion}([p objectAtIndex:0]);
  cppPair.second = {secondConversion}([p objectAtIndex:1]);
  return cppPair;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("errorCheck", getErrorCheck(data)),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("firstConversion", firstConversion.m_toCpp),
      fmt::arg("secondConversion", secondConversion.m_toCpp)));

  data.functions.push_back(fmt::format(
      R"(
NSArray* {toObjcName}({noQualifiers} const& p) {{
  NSMutableArray* objcPair = [NSMutableArray arrayWithCapacity:2];
  [objcPair addObject: {firstConversion}(p.first)];
  [objcPair addObject: {secondConversion}(p.second)];
  return objcPair;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("firstConversion", firstConversion.m_toObjc),
      fmt::arg("secondConversion", secondConversion.m_toObjc)));
}

Objc::Conversions::Conversion
getTupleElementConversions(std::vector<IR::Type> const& containedTypes) {
  Objc::Conversions::Conversion elementConversion;
  for (size_t i = 0; i < containedTypes.size(); ++i) {
    auto currentConversion =
        Objc::Conversions::getConversionContainerName(containedTypes[i]);

    elementConversion.m_toObjc +=
        fmt::format("  [objcTuple addObject: {toObjc}(std::get<{i}>(t))];\n",
                    fmt::arg("toObjc", currentConversion.m_toObjc),
                    fmt::arg("i", i));
    elementConversion.m_toCpp += fmt::format(
        "  std::get<{i}>(cppTuple) = {toCpp}([t objectAtIndex:{i}]);\n",
        fmt::arg("toCpp", currentConversion.m_toCpp),
        fmt::arg("i", i));
  }
  return elementConversion;
}

void convertTupleType(ContainerData data) {
  auto elementConversions = getTupleElementConversions(data.containedTypes);

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}(NSArray* t) {{
{errorCheck}
  {noQualifiers} cppTuple;
{elementConversions}
  return cppTuple;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("errorCheck", getErrorCheck(data)),
      fmt::arg("elementConversions", elementConversions.m_toCpp)));

  data.functions.push_back(fmt::format(
      R"(
NSArray* {toObjcName}({noQualifiers} const& t) {{
  NSMutableArray* objcTuple = [NSMutableArray arrayWithCapacity:{tupleSize}];
{elementConversions}
  return objcTuple;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("tupleSize", data.containedTypes.size()),
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("elementConversions", elementConversions.m_toObjc)));
}

void convertOptionalType(ContainerData data, std::string_view objcValueType) {
  auto valueConversion =
      Objc::Conversions::getConversionContainerName(data.containedTypes[0]);

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}({objcValueType} o) {{
  if (o) {{
    return {valueConversion}(o);
  }}
  return std::nullopt;
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("objcValueType", objcValueType),
      fmt::arg("valueConversion", valueConversion.m_toCpp)));

  data.functions.push_back(fmt::format(
      R"(
{objcValueType} {toObjcName}({noQualifiers} const& o) {{
  if (o.has_value()) {{
    return {valueConversion}(o.value());
  }}
  return nil;
}})",
      fmt::arg("objcValueType", objcValueType),
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("valueConversion", valueConversion.m_toObjc)));
}

void convertSmartPtrType(ContainerData data,
                         std::string_view objcValueType,
                         bool isSharedPtr) {
  std::string_view objcClassName = objcValueType;
  if (objcClassName.ends_with('*')) {
    objcClassName.remove_suffix(1);
  }

  data.functions.push_back(fmt::format(
      R"(
{noQualifiers} {toCppName}({objcValueType} objcClass) {{
  return {leftMove}[objcClass Tolc_getUnderlyingSmartPtr]{rightMove};
}})",
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("toCppName", data.names.m_toCpp),
      fmt::arg("leftMove", isSharedPtr ? "" : "std::move("),
      fmt::arg("rightMove", isSharedPtr ? "" : ")"),
      fmt::arg("objcValueType", objcValueType)));

  data.functions.push_back(fmt::format(
      R"(
{objcValueType} {toObjcName}({noQualifiers} cppClass) {{
  return [[{objcClassName} alloc] Tolc_initWithSmartPtr:{leftMove}cppClass{rightMove}];
}})",
      fmt::arg("objcValueType", objcValueType),
      fmt::arg("toObjcName", data.names.m_toObjc),
      fmt::arg("noQualifiers", data.noQualifiers),
      fmt::arg("leftMove", isSharedPtr ? "" : "std::move("),
      fmt::arg("rightMove", isSharedPtr ? "" : ")"),
      fmt::arg("objcClassName", objcClassName)));
}

Objc::Conversions::Conversion
containerConversion(IR::Type const& type,
                    IR::Type::Container const& container,
                    std::vector<std::string>& functions,
                    std::queue<IR::Type const*>& typesToConvert,
                    Objc::Cache& cache) {
  using IR::ContainerType;
  switch (container.m_container) {
    case ContainerType::Array:
    case ContainerType::Deque:
    case ContainerType::List:
    case ContainerType::Valarray:
    case ContainerType::Vector: {
      if (!container.m_containedTypes.empty()) {
        // The first one is the Stuff in vector<Stuff>
        typesToConvert.push(&container.m_containedTypes.front());
        return convertContainerWrapper(type,
                                       container.m_container,
                                       container.m_containedTypes,
                                       functions,
                                       cache,
                                       convertArrayType);
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::UnorderedMap:
    case ContainerType::Map: {
      if (container.m_containedTypes.size() >= 2) {
        // The first one is the Stuff, Other in map<Stuff, Other>
        typesToConvert.push(&container.m_containedTypes[0]);
        typesToConvert.push(&container.m_containedTypes[1]);
        return convertContainerWrapper(type,
                                       container.m_container,
                                       container.m_containedTypes,
                                       functions,
                                       cache,
                                       convertMapType);
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::UnorderedSet:
    case ContainerType::Set: {
      if (!container.m_containedTypes.empty()) {
        // The first one is the Stuff, Other in map<Stuff, Other>
        typesToConvert.push(&container.m_containedTypes[0]);
        return convertContainerWrapper(
            type,
            container.m_container,
            container.m_containedTypes,
            functions,
            cache,
            [isOrdered = container.m_container == ContainerType::Set](
                ContainerData data) { convertSetType(data, isOrdered); });
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::Pair: {
      if (container.m_containedTypes.size() >= 2) {
        // The first one is the Stuff, Other in pair<Stuff, Other>
        typesToConvert.push(&container.m_containedTypes[0]);
        typesToConvert.push(&container.m_containedTypes[1]);
        return convertContainerWrapper(type,
                                       container.m_container,
                                       container.m_containedTypes,
                                       functions,
                                       cache,
                                       convertPairType);
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::Tuple: {
      for (auto const& containedType : container.m_containedTypes) {
        typesToConvert.push(&containedType);
      }
      return convertContainerWrapper(type,
                                     container.m_container,
                                     container.m_containedTypes,
                                     functions,
                                     cache,
                                     convertTupleType);
    }
    case ContainerType::Optional: {
      if (!container.m_containedTypes.empty()) {
        // The first one is the Stuff in optional<Stuff>
        typesToConvert.push(&container.m_containedTypes[0]);
        return convertContainerWrapper(
            type,
            container.m_container,
            container.m_containedTypes,
            functions,
            cache,
            [objcValueName = Objc::getContainedTypeName(
                 container.m_containedTypes[0], cache.m_moduleName)](
                ContainerData data) {
              convertOptionalType(data, objcValueName);
            });
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::UniquePtr: {
      if (!container.m_containedTypes.empty()) {
        // The first one is the Stuff in unique_ptr<Stuff>
        typesToConvert.push(&container.m_containedTypes[0]);
        return convertContainerWrapper(
            type,
            container.m_container,
            container.m_containedTypes,
            functions,
            cache,
            [objcValueName = Objc::getContainedTypeName(
                 container.m_containedTypes[0], cache.m_moduleName)](
                ContainerData data) {
              convertSmartPtrType(data, objcValueName, false);
            });
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::SharedPtr: {
      if (!container.m_containedTypes.empty()) {
        if (auto userDefined = std::get_if<IR::Type::UserDefined>(
                &container.m_containedTypes[0].m_type)) {
          // Register as shared
          cache.m_sharedPtrClasses.insert(userDefined->m_representation);
        }
        // The first one is the Stuff in unique_ptr<Stuff>
        typesToConvert.push(&container.m_containedTypes[0]);
        return convertContainerWrapper(
            type,
            container.m_container,
            container.m_containedTypes,
            functions,
            cache,
            [objcValueName = Objc::getContainedTypeName(
                 container.m_containedTypes[0], cache.m_moduleName)](
                ContainerData data) {
              convertSmartPtrType(data, objcValueName, true);
            });
      }    // TODO: Handle error
      return {};
    }
    case ContainerType::MultiMap:
    case ContainerType::MultiSet:
    case ContainerType::PriorityQueue:
    case ContainerType::Queue:
    case ContainerType::Stack:
    case ContainerType::UnorderedMultiMap:
    case ContainerType::UnorderedMultiSet:
    case ContainerType::Variant:
    case ContainerType::Allocator:
    case ContainerType::EqualTo:
    case ContainerType::ForwardList:
    case ContainerType::Greater:
    case ContainerType::Hash:
    case ContainerType::Less: return {};
  }
  return {};
}

}    // namespace Objc::Conversions::Container
