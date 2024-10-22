#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.hpp"
#include <vector>

namespace Builders {
/**
* Convert the IRProxy::Enum to IR::Enum and add them to the structure
*/
void buildEnums(const std::vector<IRProxy::Enum>& enums,
                IR::Namespace& globalNamespace);

}    // namespace Builders
