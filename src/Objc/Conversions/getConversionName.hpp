#pragma once

#include "Objc/Conversions/conversion.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Objc::Conversions {

/**
* The function name to take this enum to the equivalent Objective-C type
* and the other way around
* Pass an empty conversionNamespace to have the function without the namespace
*/
Objc::Conversions::Conversion
getEnumConversionNames(std::string const& moduleName,
                       std::string const& fullyQualifiedEnumName,
                       std::string const& conversionNamespace = "");

/**
* The function name to take this base type to the equivalent Objective-C type
* and the other way around
* Pass an empty conversionNamespace to have the function without the namespace
*/
Objc::Conversions::Conversion
getConversionBaseName(IR::BaseType baseType,
                      std::string const& conversionNamespace = "");

/**
* The function name to take this container type to the equivalent Objective-C type
* and the other way around
* Pass an empty conversionNamespace to have the function without the namespace
*/
Objc::Conversions::Conversion
getConversionContainerName(IR::Type const& containerType,
                           std::string const& conversionNamespace = "");
}
