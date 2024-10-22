#pragma once

#include <string>
#include <vector>

namespace Helpers::Utils {
/**
* Create and return a vector of the combined elements of v0 and v1
*/
std::vector<std::string> combine(std::vector<std::string> const& v0,
                                 std::vector<std::string> const& v1);
}
