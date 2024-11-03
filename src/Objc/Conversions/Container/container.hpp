#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {
Objc::Conversions::Conversion
containerConversion(IR::Type const& type,
                    IR::Type::Container const& container,
                    std::vector<std::string>& functions,
                    std::queue<IR::Type const*>& typesToConvert,
                    Objc::Cache& cache);
}
