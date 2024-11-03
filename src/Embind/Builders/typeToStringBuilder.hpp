#pragma once
#include <IR/ir.hpp>

#include <string>
#include <vector>

namespace Embind::Builders {

/**
* Transforms a IR::Type to a _ separated version of the type
* Ex:
*   int -> int
*   std::vector<int> -> vector_int
*   std::map<int, std::string> -> map_int_string
*   std::map<UserDefined, int> -> map_UserDefined_int
*/
std::string buildTypeString(IR::Type const& t, std::string const& separator);

/**
* Call buildTypeString on a number of parameters and join them
*/
std::string getSeparatedTypeString(std::vector<IR::Type> const& parameters,
                                   std::string const& separator);

/**
* std::vector<int> -> "int"
* std::map<int, double> -> "int, double"
*/
std::string getTemplateParameters(IR::Type const& type);
}    // namespace Embind::Builders
