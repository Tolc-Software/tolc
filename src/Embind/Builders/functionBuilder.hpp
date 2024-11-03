#pragma once

#include "Embind/Proxy/function.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <IR/ir.hpp>

#include <optional>

namespace Embind::Builders {

/**
* Transforms a IR::Function to a Embind::Proxy::Function
*/
std::optional<Embind::Proxy::Function>
buildFunction(IR::Function const& fun, Embind::Proxy::TypeInfo& typeInfo);
}    // namespace Embind::Builders
