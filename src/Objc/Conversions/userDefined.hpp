#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <string_view>

namespace Objc::Conversions {
Objc::Conversions::Conversion
getUserDefinedConversionNames(std::string_view fullyQualifiedName,
                              std::string_view ns);
}
