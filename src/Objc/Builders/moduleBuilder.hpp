#pragma once

#include "Objc/Proxy/class.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Objc::Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<Objc::Proxy::Class> buildModule(IR::Namespace const& ns,
                                              Objc::Cache& cache);
}    // namespace Objc::Builders
