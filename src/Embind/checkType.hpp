#pragma once

#include "Embind/Proxy/typeInfo.hpp"

#include <IR/ir.hpp>

#include <queue>
#include <set>
#include <string>

namespace Embind {

// Fill TypeInfo related to input type
// Should be called once per type in the whole interface
void checkType(IR::Type const& type, Embind::Proxy::TypeInfo& info);

}    // namespace Embind
