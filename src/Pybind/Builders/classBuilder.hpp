#pragma once

#include "Pybind/Proxy/class.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Pybind::Builders {

/**
* Transforms a IR::Struct to a Pybind::Proxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<Pybind::Proxy::Class>
buildClass(IR::Struct const& s, Pybind::Proxy::TypeInfo& typeInfo);
}    // namespace Pybind::Builders
