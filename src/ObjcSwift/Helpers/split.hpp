#pragma once

#include <deque>
#include <string>
#include <string_view>

namespace ObjcSwift::Helpers {
std::deque<std::string> split(std::string_view s, std::string_view delimiter);
}
