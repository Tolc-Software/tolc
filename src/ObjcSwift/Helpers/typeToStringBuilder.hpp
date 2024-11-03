#pragma once
#include <IR/ir.hpp>
#include <string>

namespace ObjcSwift::Helpers {

/**
* Transforms a IR::Type to a string version of the type
* Ex:
*   int -> Int
*   std::vector<int> -> VectorInt
*   std::map<int, std::string> -> MapIntString
*   std::map<UserDefined, int> -> MapUserDefinedInt
*/
std::string buildTypeString(IR::Type const& t);
}    // namespace ObjcSwift::Helpers
