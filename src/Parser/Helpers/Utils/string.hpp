#pragma once

#include <string>
#include <string_view>

namespace Helpers::Utils {
/**
* Remove substr from str and return the result.
* If str does not contain substr, return str.
*/
std::string removeSubString(std::string str, std::string_view substr);
}
