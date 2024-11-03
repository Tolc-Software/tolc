#pragma once

#include "Pybind/Proxy/attribute.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>

namespace Pybind::Builders {

/**
* Transforms a IR::Variable to a Pybind::Proxy::Attribute
*/
Pybind::Proxy::Attribute buildAttribute(std::string const& parentNamespace,
                                        IR::Variable const& v,
                                        Pybind::Proxy::TypeInfo& typeInfo);
}    // namespace Pybind::Builders
