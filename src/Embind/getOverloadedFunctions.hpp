#pragma once

#include <IR/ir.hpp>

#include <set>
#include <string>
#include <vector>

namespace Embind {

// Go through functions and return the representations of the functions that are overloaded
// i.e. exists twice in functions
std::set<std::string>
getOverloadedFunctions(std::vector<IR::Function> const& functions);
}    // namespace Embind
