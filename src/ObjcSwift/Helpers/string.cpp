#include "ObjcSwift/Helpers/string.hpp"
#include <string>
#include <string_view>

namespace ObjcSwift::Helpers {
std::string
removeSubString(std::string str, std::string_view substr, bool reverse) {
	// Search for the substring in string
	size_t pos = std::string::npos;
	if (reverse) {
		pos = str.rfind(substr);
	} else {
		pos = str.find(substr);
	}

	if (pos != std::string::npos) {
		// If found then erase it from string
		str.erase(pos, substr.length());
	}
	return str;
}
}    // namespace ObjcSwift::Helpers
