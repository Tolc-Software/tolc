#pragma once
#include <IR/ir.hpp>
#include <set>
#include <string>
#include <vector>

namespace Pybind {

// Go through functions and return the representations of the functions that are overloaded
// i.e. exists twice in functions
std::set<std::string>
getOverloadedFunctions(std::vector<IR::Function> const& functions);

std::set<std::string> getOverloadedFunctions(
    std::vector<std::pair<IR::Operator, IR::Function>> const& functions);
}    // namespace Pybind
