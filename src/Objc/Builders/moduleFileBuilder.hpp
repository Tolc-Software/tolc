#pragma once

#include "Objc/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Objc::Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
std::optional<Objc::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& moduleName);
}    // namespace Objc::Builders
