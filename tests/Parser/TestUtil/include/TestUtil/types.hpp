#pragma once

#include "IR/ir.hpp"
#include "TestUtil/finders.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include <variant>

namespace TestUtil {

IR::Type::Container const* getContainer(IR::Type const& type) {
	if (auto container = std::get_if<IR::Type::Container>(&type.m_type)) {
		return container;
	}
	return nullptr;
}

std::vector<TestUtil::AccessModifier> getAccessModifiers() {
	using TestUtil::AccessModifier;
	return {
	    AccessModifier::Public,
	    AccessModifier::Private,
	    AccessModifier::Protected,
	};
}

std::vector<IR::BaseType> getTypes() {
	using IR::BaseType;
	return {
	    BaseType::Bool,
	    BaseType::Char,
	    BaseType::Char16_t,
	    BaseType::Char32_t,
	    BaseType::Complex,
	    BaseType::Double,
	    BaseType::FilesystemPath,
	    BaseType::Float,
	    BaseType::Int,
	    BaseType::LongDouble,
	    BaseType::LongInt,
	    BaseType::LongLongInt,
	    BaseType::ShortInt,
	    BaseType::SignedChar,
	    BaseType::String,
	    BaseType::StringView,
	    BaseType::UnsignedChar,
	    BaseType::UnsignedInt,
	    BaseType::UnsignedLongInt,
	    BaseType::UnsignedLongLongInt,
	    BaseType::UnsignedShortInt,
	    BaseType::Void,
	    BaseType::Wchar_t,
	};
}

std::string getAsString(TestUtil::AccessModifier am) {
	using TestUtil::AccessModifier;
	switch (am) {
		case AccessModifier::Public: return "public";
		case AccessModifier::Private: return "private";
		case AccessModifier::Protected: return "protected";
	}
	return "";
}

std::string getAsString(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "bool";
		case BaseType::Char16_t: return "char16_t";
		case BaseType::Char32_t: return "char32_t";
		case BaseType::Char: return "char";
		case BaseType::Complex: return "std::complex<int>";
		case BaseType::Double: return "double";
		case BaseType::FilesystemPath: return "std::filesystem::path";
		case BaseType::Float: return "float";
		case BaseType::Int: return "int";
		case BaseType::LongDouble: return "long double";
		case BaseType::LongInt: return "long int";
		case BaseType::LongLongInt: return "long long int";
		case BaseType::ShortInt: return "short int";
		case BaseType::SignedChar: return "signed char";
		case BaseType::String: return "std::string";
		case BaseType::StringView: return "std::string_view";
		case BaseType::UnsignedChar: return "unsigned char";
		case BaseType::UnsignedInt: return "unsigned int";
		case BaseType::UnsignedLongInt: return "unsigned long int";
		case BaseType::UnsignedLongLongInt: return "unsigned long long int";
		case BaseType::UnsignedShortInt: return "unsigned short int";
		case BaseType::Void: return "void";
		case BaseType::Wchar_t: return "wchar_t";
	}
	return "";
}

std::optional<IR::BaseType> getIRFromString(std::string const& type) {
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
		// Either in LLVM lib, the one provided by Apple, or on Windows
	} else if (
	    type == "class std::__cxx11::basic_string<char>" ||
	    type ==
	        "class std::__1::basic_string<char, struct std::__1::char_traits<char>, class std::__1::allocator<char> >" ||
	    type ==
	        "class std::basic_string<char, struct std::char_traits<char>, class std::allocator<char> >") {
		return BaseType::String;
	} else if (type == "void") {
		return BaseType::Void;
	}
	return {};
}

std::string getIncludesIfNeeded(IR::BaseType type) {
	std::string include = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::FilesystemPath:
			include = "#include <filesystem>\n";
			break;
		case BaseType::String: include = "#include <string>\n"; break;
		case BaseType::StringView: include = "#include <string_view>\n"; break;
		case BaseType::Complex: include = "#include <complex>\n"; break;
		default: break;
	}
	return include;
}

std::string getValidReturnForType(IR::BaseType type) {
	std::string validReturn = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "true";
		case BaseType::Char16_t:
		case BaseType::Char32_t:
		case BaseType::SignedChar:
		case BaseType::UnsignedChar:
		case BaseType::Wchar_t:
		case BaseType::Char: return "'c'";
		case BaseType::Double:
		case BaseType::LongDouble:
		case BaseType::Float: return "0.0";
		case BaseType::Complex:
		case BaseType::Int:
		case BaseType::LongInt:
		case BaseType::LongLongInt:
		case BaseType::ShortInt:
		case BaseType::UnsignedLongInt:
		case BaseType::UnsignedLongLongInt:
		case BaseType::UnsignedShortInt:
		case BaseType::UnsignedInt: return "0";
		case BaseType::FilesystemPath:
		case BaseType::String:
		case BaseType::StringView: return "\"HelloWorld\"";
		case BaseType::Void: return "";
	}
	return "";
}

/**
* Return all (hopefully) valid base types as strings. Optionally exclude some by input
*/
std::vector<std::string>
getBaseTypes(std::vector<std::string> const& excluding = {}) {
	std::vector<std::string> baseTypes = {"bool",
	                                      "char16_t",
	                                      "char32_t",
	                                      "char",
	                                      "double",
	                                      "float",
	                                      "int",
	                                      "long double",
	                                      "long",
	                                      "long long",
	                                      "short",
	                                      "signed char",
	                                      "std::string",
	                                      "unsigned char",
	                                      "unsigned int",
	                                      "unsigned long",
	                                      "unsigned long long",
	                                      "unsigned short",
	                                      "void",
	                                      "wchar_t"};
	for (auto e : excluding) {
		baseTypes.erase(std::remove(baseTypes.begin(), baseTypes.end(), e),
		                baseTypes.end());
	}
	return baseTypes;
}

}    // namespace TestUtil
