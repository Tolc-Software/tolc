#pragma once

#include "IR/ir.hpp"
#include "IRProxy/IRData.hpp"
#include <map>
#include <string>
#include <vector>

namespace Builders {
/**
  * Add the variables to the structs
  */
void addMemberVariables(
    std::vector<IRProxy::Struct>& structs,
    std::map<std::string, std::vector<IRProxy::MemberVariable>>&
        memberVariables);

/**
  * Convert the Helpers::Struct to IR::Struct and add them to the structure
  */
void buildStructs(std::vector<IRProxy::Struct>& structs,
                  IR::Namespace& globalNamespace);
}    // namespace Builders
