#pragma once

#include "Objc/Proxy/function.hpp"
#include "Swift/Proxy/function.hpp"
#include <string>

namespace Swift::Builders {

/**
* Transforms a Objc::Proxy::Function to a Swift::Proxy::Function
*/
Swift::Proxy::Function buildFunction(Objc::Proxy::Function const& objcFunction,
                                     std::string const& libraryName);
}    // namespace Swift::Builders
