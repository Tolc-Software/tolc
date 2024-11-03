#include "Swift/Builders/typeBuilder.hpp"
#include "Objc/Proxy/type.hpp"
#include <IR/ir.hpp>
#include <string>
#include <variant>

namespace Swift::Builders {

namespace {

std::string toSwiftType(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "Bool";
		case BaseType::Char16_t: return "Character";
		case BaseType::Char32_t: return "Character";
		case BaseType::Char: return "Character";
		case BaseType::Complex: return "complex";
		case BaseType::Double: return "Double";
		case BaseType::FilesystemPath: return "String";
		case BaseType::Float: return "Float";
		case BaseType::Int: return "Int32";
		case BaseType::LongDouble: return "Double";
		case BaseType::LongInt: return "Int64";
		case BaseType::LongLongInt: return "Int64";
		case BaseType::ShortInt: return "Int8";
		case BaseType::SignedChar: return "Character";
		case BaseType::String: return "String";
		case BaseType::StringView: return "String";
		case BaseType::UnsignedChar: return "Character";
		case BaseType::UnsignedInt: return "UInt";
		case BaseType::UnsignedLongInt: return "UInt";
		case BaseType::UnsignedLongLongInt: return "UInt";
		case BaseType::UnsignedShortInt: return "UInt";
		case BaseType::Void: return "Void";
		case BaseType::Wchar_t: return "Character";
	}
	return "";
}

}    // namespace

std::string buildType(Objc::Proxy::Type const& type) {
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_cppType->m_type)) {
		return toSwiftType(baseType->m_base);
	}
	return "";
}
}    // namespace Swift::Builders
