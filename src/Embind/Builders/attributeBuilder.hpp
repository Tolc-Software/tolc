#pragma once

#include "Embind/Proxy/attribute.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <IR/ir.hpp>

#include <string>

namespace Embind::Builders {

/**
* Transforms a IR::Variable to a Embind::Proxy::Attribute
*/
Embind::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                        IR::Variable const& v,
                                        Embind::Proxy::TypeInfo& typeInfo);
}    // namespace Embind::Builders
