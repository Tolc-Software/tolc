#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <map>
#include <string>
#include <vector>

namespace Builders {
/**
  * Build the structure between namespaces (parent - children)
  * and return the root namespace (always the global namespace)
  */
IR::Namespace
buildNamespaceStructure(std::vector<IRProxy::Namespace> const& namespaces);

/**
  * Add the variables to the corresponding namespaces
  */
void addGlobalVariables(
    std::map<std::string, std::vector<IR::Variable>> const& variables,
    IR::Namespace& globalNS);
}    // namespace Builders
