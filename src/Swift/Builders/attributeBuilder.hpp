#pragma once

#include "Objc/Proxy/attribute.hpp"
#include "Swift/Proxy/attribute.hpp"
#include <IR/ir.hpp>
#include <string>

namespace Swift::Builders {

/**
* Transforms a Objc::Proxy::Attribute to a Swift::Proxy::Attribute
*/
Swift::Proxy::Attribute
buildAttribute(Objc::Proxy::Attribute const& objcAttribute,
               std::string const& callFrom);
}    // namespace Swift::Builders
