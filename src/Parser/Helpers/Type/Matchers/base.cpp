#include "Helpers/Type/Matchers/base.hpp"
#include <ctre.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {
// Either in LLVM lib, the one provided by Apple, or on Windows
static constexpr auto stringPattern =
    ctll::fixed_string {"(struct|class)? ?std(::__1)?::basic_string<.*?>"};

constexpr auto matchString(std::string_view sv) noexcept {
	return ctre::match<stringPattern>(sv);
}

static constexpr auto stringViewPattern =
    ctll::fixed_string {"(struct|class)? ?std(::__1)?::basic_string_view<.*?>"};

constexpr auto matchStringView(std::string_view sv) noexcept {
	return ctre::match<stringViewPattern>(sv);
}

static constexpr auto filesystemPathPattern =
    ctll::fixed_string {"(struct|class)? ?std(::__1)?::filesystem::path"};

constexpr auto matchFilesystemPath(std::string_view sv) noexcept {
	return ctre::match<filesystemPathPattern>(sv);
}

static constexpr auto complexPattern =
    ctll::fixed_string {"(struct|class)? ?std(::__1)?::complex<.*?>"};

constexpr auto matchComplex(std::string_view sv) noexcept {
	return ctre::match<complexPattern>(sv);
}

std::optional<IR::BaseType> getBaseType(std::string_view type) {
	using IR::BaseType;
	// Types and alternate types are listed in https://en.cppreference.com/w/cpp/language/types
	if (type == "bool" || type == "_Bool") {
		return BaseType::Bool;
	} else if (type == "char16_t") {
		return BaseType::Char16_t;
	} else if (type == "char32_t") {
		return BaseType::Char32_t;
	} else if (type == "signed char") {
		return BaseType::SignedChar;
	} else if (type == "unsigned char") {
		return BaseType::UnsignedChar;
	} else if (type == "wchar_t") {
		return BaseType::Wchar_t;
	} else if (type == "char") {
		return BaseType::Char;
	} else if (type == "double") {
		return BaseType::Double;
	} else if (type == "long double") {
		return BaseType::LongDouble;
	} else if (type == "float") {
		return BaseType::Float;
	} else if (type == "int" || type == "signed int" || type == "signed") {
		return BaseType::Int;
	} else if (type == "long" || type == "long int" || type == "signed long" ||
	           type == "signed long int") {
		return BaseType::LongInt;
	} else if (type == "long long" || type == "long long int" ||
	           type == "signed long long" || type == "signed long long int") {
		return BaseType::LongLongInt;
	} else if (type == "short" || type == "short int" ||
	           type == "signed short" || type == "signed short int") {
		return BaseType::ShortInt;
	} else if (type == "unsigned long" || type == "unsigned long int") {
		return BaseType::UnsignedLongInt;
	} else if (type == "unsigned long long" ||
	           type == "unsigned long long int") {
		return BaseType::UnsignedLongLongInt;
	} else if (type == "unsigned short" || type == "unsigned short int") {
		return BaseType::UnsignedShortInt;
	} else if (type == "unsigned" || type == "unsigned int") {
		return BaseType::UnsignedInt;
	} else if (type == "std::string" || matchString(type)) {
		return BaseType::String;
	} else if (type == "std::string_view" || matchStringView(type)) {
		return BaseType::StringView;
	} else if (type == "std::filesystem::path" || matchFilesystemPath(type)) {
		return BaseType::FilesystemPath;
	} else if (matchComplex(type)) {
		return BaseType::Complex;
	} else if (type == "void") {
		return BaseType::Void;
	}
	return {};
}

}    // namespace Helpers::Type::Matchers
