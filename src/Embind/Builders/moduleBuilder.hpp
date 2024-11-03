#pragma once

#include "Embind/Builders/functionBuilder.hpp"
#include "Embind/Proxy/module.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <IR/ir.hpp>

#include <optional>

namespace Embind::Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<Embind::Proxy::Module>
buildModule(IR::Namespace const& ns, Embind::Proxy::TypeInfo& typeInfo);
}    // namespace Embind::Builders
