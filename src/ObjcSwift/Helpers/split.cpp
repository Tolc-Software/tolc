#include "ObjcSwift/Helpers/split.hpp"
#include <deque>
#include <string>
#include <string_view>

namespace ObjcSwift::Helpers {
std::deque<std::string> split(std::string_view s, std::string_view delimiter) {
	std::deque<std::string> splitStrings;
	std::size_t last = 0;
	std::size_t next = 0;
	while ((next = s.find(delimiter, last)) != std::string::npos) {
		splitStrings.push_back(std::string(s.substr(last, next - last)));
		last = next + delimiter.length();
	}
	splitStrings.push_back(std::string(s.substr(last)));
	return splitStrings;
}
}    // namespace ObjcSwift::Helpers
