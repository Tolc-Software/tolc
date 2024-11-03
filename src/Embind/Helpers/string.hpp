#pragma once

#include <string>
#include <string_view>

namespace Embind::Helpers {
/**
* Remove substr from str and return the result.
* If str does not contain substr, return str.
*/
std::string removeSubString(std::string str, std::string_view substr);

/**
* Trims from left until c
*/
std::string trimLeft(std::string str, char c);

/**
* Trims from right until c
*/
std::string trimRight(std::string str, char c);

/**
* Extracts substring from the right until the character
*/
std::string extractRightUntil(std::string str, char c);
}    // namespace Embind::Helpers
