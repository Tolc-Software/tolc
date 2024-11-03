#pragma once

#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <string>

namespace Pybind {

// Fill TypeInfo related to input type
// Should be called once per type in the whole interface
void checkType(IR::Type const& type, Pybind::Proxy::TypeInfo& info);

}    // namespace Pybind
