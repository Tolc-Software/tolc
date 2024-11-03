#pragma once

#include "Pybind/Proxy/enum.hpp"
#include <IR/ir.hpp>

namespace Pybind::Builders {

/**
* Transforms a IR::Enum to a Pybind::Proxy::Enum
*/
Pybind::Proxy::Enum buildEnum(IR::Enum const& s);
}    // namespace Pybind::Builders
