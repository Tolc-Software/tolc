#pragma once
#include <IR/ir.hpp>
#include <string>

namespace Pybind::Builders {

/**
* Transforms a IR::Type to a _ separated version of the type
* Ex:
*   int -> int
*   std::vector<int> -> vector_int
*   std::map<int, std::string> -> map_int_string
*   std::map<UserDefined, int> -> map_UserDefined_int
*/
std::string buildTypeString(IR::Type const& t);
}    // namespace Pybind::Builders
