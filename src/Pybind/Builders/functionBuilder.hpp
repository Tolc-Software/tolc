#pragma once

#include "Pybind/Proxy/function.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Pybind::Builders {

/**
* Transforms a IR::Function to a Pybind::Proxy::Function
* Fails if any of the arguments are std::unique_ptr (https://pybind11.readthedocs.io/en/stable/advanced/smart_ptrs.html)
*/
std::optional<Pybind::Proxy::Function>
buildFunction(IR::Function const& fun, Pybind::Proxy::TypeInfo& typeInfo);
}    // namespace Pybind::Builders
