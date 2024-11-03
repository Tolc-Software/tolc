#pragma once

#include "Embind/Proxy/class.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <IR/ir.hpp>

#include <optional>

namespace Embind::Builders {

/**
* Transforms a IR::Struct to a Embind::Proxy::Class
* NOTE: If the struct does not have an explicit constructor,
*       the default constructor is added
* Fails if any member functions takes unique_ptr as an argument
*/
std::optional<Embind::Proxy::Class>
buildClass(IR::Struct const& s, Embind::Proxy::TypeInfo& typeInfo);
}    // namespace Embind::Builders
