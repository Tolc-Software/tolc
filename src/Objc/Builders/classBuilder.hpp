#pragma once

#include "Objc/Proxy/class.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>

namespace Objc::Builders {

/**
* Transforms a IR::Struct to a Objc::Proxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
*/
Objc::Proxy::Class buildClass(IR::Struct const& cppClass, Objc::Cache& cache);
}    // namespace Objc::Builders
