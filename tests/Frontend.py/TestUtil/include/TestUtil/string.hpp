#pragma once

#include <string>

namespace TestUtil {
bool contains(std::string const& str, std::string const& substr) {
	return str.find(substr) != std::string::npos;
}
}    // namespace TestUtil
