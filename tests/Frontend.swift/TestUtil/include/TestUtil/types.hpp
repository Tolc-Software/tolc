#pragma once

#include "IR/ir.hpp"

namespace TestUtil {

IR::Type getType() {
	IR::Type t;
	IR::Type::Value v;
	v.m_base = IR::BaseType::Int;
	t.m_type = v;
	t.m_isConst = false;
	t.m_isReference = false;
	t.m_numPointers = 0;
	t.m_representation = "int";
	return t;
}

IR::Type getVector() {
	auto t = getType();
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Vector;
	c.m_containedTypes.push_back(getType());
	t.m_type = c;
	return t;
}

IR::Type getMap() {
	auto t = getType();
	IR::Type::Container c;
	c.m_container = IR::ContainerType::Map;
	c.m_containedTypes.push_back(getType());
	c.m_containedTypes.push_back(getType());
	t.m_type = c;
	return t;
}

IR::Type::Container* getContainer(IR::Type& t) {
	if (auto c = std::get_if<IR::Type::Container>(&t.m_type)) {
		return c;
	}
	return nullptr;
}

std::vector<IR::BaseType> getTypes() {
	using IR::BaseType;
	return {BaseType::Bool,
	        BaseType::Char,
	        BaseType::Char16_t,
	        BaseType::Char32_t,
	        BaseType::Double,
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
	        BaseType::Wchar_t};
}

std::string getAsString(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "bool";
		case BaseType::Char16_t: return "char16_t";
		case BaseType::Char32_t: return "char32_t";
		case BaseType::Char: return "char";
		case BaseType::Complex: return "std::complex<double>";
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

std::string getIncludesIfNeeded(IR::BaseType type) {
	std::string include = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::String: include = "#include <string>\n";
		case BaseType::StringView: include = "#include <string_view>\n";
		default: break;
	}
	return include;
}

std::string getValidReturnForType(IR::BaseType type) {
	std::string validReturn = "";
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "true";
		case BaseType::Char16_t: return "'c'";
		case BaseType::Char32_t: return "'c'";
		case BaseType::Char: return "'c'";
		case BaseType::Complex: return "0";
		case BaseType::Double: return "0";
		case BaseType::FilesystemPath: return "\"path\"";
		case BaseType::Float: return "0";
		case BaseType::Int: return "0";
		case BaseType::LongDouble: return "0";
		case BaseType::LongInt: return "0";
		case BaseType::LongLongInt: return "0";
		case BaseType::ShortInt: return "0";
		case BaseType::SignedChar: return "'c'";
		case BaseType::String: return "\"HelloWorld\"";
		case BaseType::StringView: return "\"HelloWorld\"";
		case BaseType::UnsignedChar: return "'c'";
		case BaseType::UnsignedInt: return "0";
		case BaseType::UnsignedLongInt: return "0";
		case BaseType::UnsignedLongLongInt: return "0";
		case BaseType::UnsignedShortInt: return "0";
		case BaseType::Void: return "";
		case BaseType::Wchar_t: return "'c'";
	}
	return "";
}

}    // namespace TestUtil
