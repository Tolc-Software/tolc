#include "Embind/Helpers/string.hpp"

#include <string>
#include <string_view>

namespace Embind::Helpers {
std::string removeSubString(std::string str, std::string_view substr) {
  // Search for the substring in string
  size_t pos = str.find(substr);

  if (pos != std::string::npos) {
    // If found then erase it from string
    str.erase(pos, substr.length());
  }
  return str;
}

std::string trimLeft(std::string str, char c) {
  if (auto it = std::find_if(str.begin(),
                             str.end(),
                             [c](char ch) {
                               // Stop at c
                               return ch == c;
                             });
      it != str.end()) {
    // Move forward to erase the actual character aswell
    ++it;
    str.erase(str.begin(), it);
  } else {
    // Erase all of it
    str.erase(str.begin(), it);
  }
  return str;
}

std::string trimRight(std::string str, char c) {
  if (auto it = std::find_if(str.rbegin(),
                             str.rend(),
                             [c](char ch) {
                               // Stop at c
                               return ch == c;
                             })
                    .base();
      it != str.begin()) {
    // Move forward to erase the actual character aswell
    --it;
    str.erase(it, str.end());
  } else {
    // Erase all of it
    str.erase(it, str.end());
  }
  return str;
}

std::string extractRightUntil(std::string str, char c) {
  if (auto it = std::find_if(str.rbegin(),
                             str.rend(),
                             [c](char ch) {
                               // Stop at c
                               return ch == c;
                             })
                    .base();
      it != str.begin()) {
    // Move forward to erase the actual character aswell
    return std::string(it, str.end());
  } else {
    // Erase all of it
    return "";
  }
}
}    // namespace Embind::Helpers
