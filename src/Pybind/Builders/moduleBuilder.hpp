#pragma once

#include "Pybind/Builders/functionBuilder.hpp"
#include "Pybind/Proxy/module.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>

namespace Pybind::Builders {

/**
* Build proxy module from a namespace
* NOTE: Does not traverse the structure,
*       but adds the children of ns as submodules
* Fails if any functions takes unique_ptr as an argument
*/
std::optional<Pybind::Proxy::Module>
buildModule(IR::Namespace const& ns,
            std::string const& rootModuleName,
            Pybind::Proxy::TypeInfo& typeInfo);
}    // namespace Pybind::Builders
