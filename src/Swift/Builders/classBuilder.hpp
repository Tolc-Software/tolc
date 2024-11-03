#pragma once

#include "Objc/Proxy/class.hpp"
#include "Swift/Proxy/class.hpp"
#include <string>

namespace Swift::Builders {

/**
* Transforms a Objc::Proxy::Class to a Swift::Proxy::Class
*/
Swift::Proxy::Class buildClass(Objc::Proxy::Class const& objcClass,
                               std::string const& moduleName);
}    // namespace Swift::Builders
