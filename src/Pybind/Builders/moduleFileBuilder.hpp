#pragma once

#include "Pybind/Proxy/moduleFile.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Pybind::Builders {

/**
* Goes through the given namespace downwards and creates a moduleFile from it
*/
std::optional<Pybind::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& moduleName);
}    // namespace Pybind::Builders
