#pragma once

#include "Swift/Proxy/enum.hpp"
#include <IR/ir.hpp>

namespace Swift::Builders {

/**
* Transforms a IR::Enum to a Swift::Proxy::Enum
*/
Swift::Proxy::Enum buildEnum(IR::Enum const& s);
}    // namespace Swift::Builders
