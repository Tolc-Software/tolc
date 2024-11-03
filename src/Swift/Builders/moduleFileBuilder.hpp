#pragma once

#include "Objc/Proxy/structure.hpp"
#include "Swift/Proxy/moduleFile.hpp"
#include <optional>

namespace Swift::Builders {

/**
* Goes through the given structures and creates a moduleFile from it
*/
std::optional<Swift::Proxy::ModuleFile>
buildModuleFile(std::vector<Objc::Proxy::Structure const*> const& structures,
                std::string const& moduleName);
}    // namespace Swift::Builders
