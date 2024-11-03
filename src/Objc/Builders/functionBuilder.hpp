#pragma once

#include "Objc/Proxy/function.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Objc::Builders {

/**
* Transforms a IR::Function to a Objc::Proxy::Function
*/
Objc::Proxy::Function buildFunction(std::string const& objcClass,
                                    std::string const& cppClass,
                                    IR::Function const& cppFunction,
                                    Objc::Cache& cache,
                                    bool isConstructor = false,
                                    bool isOverloaded = false);
}    // namespace Objc::Builders
