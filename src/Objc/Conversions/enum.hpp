#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <array>
#include <string>
#include <vector>

namespace Objc::Conversions {

void setEnumConversions(IR::Enum const& cppEnum,
                        Objc::Proxy::Enum const& objcEnum,
                        Objc::Cache& cache);
}    // namespace Objc::Conversions
