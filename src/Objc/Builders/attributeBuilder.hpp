#pragma once

#include "Objc/Proxy/attribute.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Objc::Builders {

/**
* Transforms a IR::Variable to a Objc::Proxy::Attribute
*/
Objc::Proxy::Attribute buildAttribute(std::string const& objcClass,
                                      std::string const& cppClass,
                                      IR::Variable const& v,
                                      Objc::Cache& cache);
}    // namespace Objc::Builders
