#include "Helpers/Type/Matchers/function.hpp"
#include <ctre.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {
static constexpr auto stdFunctionPattern =
    ctll::fixed_string {"(struct|class) std(::__1)?::function<.*?>"};

constexpr auto matchStdFunction(std::string_view sv) noexcept {
	return ctre::match<stdFunctionPattern>(sv);
}

// Example:
//   void (int)
//   ".*?"       -  Matches the return type
//   " \(.*?\)"  -  Matches the space and then a number of arguments
static constexpr auto cFunctionPattern = ctll::fixed_string {R"(.*? \(.*?\))"};

constexpr auto matchCFunction(std::string_view sv) noexcept {
	return ctre::match<cFunctionPattern>(sv);
}
std::optional<IR::Type::Function> getFunctionType(std::string_view type) {
	// Can either be "std::function<...>" or C-style function "void(bool)"
	if (matchStdFunction(type) || matchCFunction(type)) {
		IR::Type::Function f;
		f.m_representation = type;
		return f;
	}
	return {};
}

}    // namespace Helpers::Type::Matchers
