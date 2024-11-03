#pragma once

#include "Embind/Proxy/enum.hpp"

#include <IR/ir.hpp>

namespace Embind::Builders {

/**
* Transforms a IR::Enum to a Embind::Proxy::Enum
*/
Embind::Proxy::Enum buildEnum(IR::Enum const& s);
}    // namespace Embind::Builders
