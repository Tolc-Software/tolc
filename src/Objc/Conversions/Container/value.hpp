#pragma once

#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {

/**
* Creates a conversion function from the
* base type 'type' and adds it to functions
* if it's not found in the cache
*/
void valueConversion(IR::Type const& type,
                     IR::BaseType baseType,
                     std::vector<std::string>& functions,
                     Objc::Cache& cache);

}    // namespace Objc::Conversions::Container
