#include "Pybind/Helpers/string.hpp"
#include <string>
#include <string_view>

namespace Pybind::Helpers {
std::string removeSubString(std::string str, std::string_view substr) {
	// Search for the substring in string
	size_t pos = str.find(substr);

	if (pos != std::string::npos) {
		// If found then erase it from string
		str.erase(pos, substr.length());
	}
	return str;
}
}    // namespace Pybind::Helpers
