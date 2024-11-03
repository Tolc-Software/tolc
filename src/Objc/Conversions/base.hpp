#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <array>
#include <string>
#include <vector>

namespace Objc::Conversions {

// {convert from, convert to}
// {function name, function}
Conversion getBaseTypeConversions(IR::BaseType baseType, Objc::Cache& cache);
}    // namespace Objc::Conversions
