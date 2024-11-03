#pragma once

#include "Objc/Proxy/class.hpp"
#include "Swift/Proxy/module.hpp"
#include "Swift/cache.hpp"
#include <vector>

namespace Swift::Builders {

/**
* Build modules from a Objc class
* May contain multiple modules since empty namespaces are not converted in Objc
* Meaning there will be MyLib.Hello.Inner where only MyLib and Inner are defined in Objc.
* In Swift we need all three
*/
std::vector<Swift::Proxy::Module>
buildModules(Objc::Proxy::Class const& objcClass, Swift::Cache& cache);
}    // namespace Swift::Builders
