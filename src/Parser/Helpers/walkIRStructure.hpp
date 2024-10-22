#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <deque>
#include <string>
#include <variant>

namespace Helpers {

/**
* @brief: Go through the path directed by path and return the last node.
*
* @param: std::deque<std::pair<std::string, IRProxy::Structure>> path,
*         IR::Namespace& globalNamespace
*
* @return: std::variant<IR::Namespace*, IR::Struct*>
*/
std::variant<IR::Namespace*, IR::Struct*> walkPathThroughStructure(
    std::deque<std::pair<std::string, IRProxy::Structure>> path,
    IR::Namespace& globalNamespace);

}    // namespace Helpers
