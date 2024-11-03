#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/userDefined.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include <fmt/format.h>
#include <string>
#include <variant>

namespace Objc::Conversions {

Objc::Conversions::Conversion
getEnumConversionNames(std::string const& moduleName,
                       std::string const& fullyQualifiedEnumName,
                       std::string const& conversionNamespace) {
	Conversion names;
	auto objcEnum = Objc::getEnumName(fullyQualifiedEnumName, moduleName);
	names.m_toCpp = fmt::format("convertEnum{}ToCpp", objcEnum);
	names.m_toObjc = fmt::format("convertEnum{}ToObjc", objcEnum);

	return addNamespace(names, conversionNamespace);
}

Objc::Conversions::Conversion
getConversionBaseName(IR::BaseType baseType,
                      std::string const& conversionNamespace) {
	Conversion names;
	names.m_toCpp = "";
	names.m_toObjc = "";
	using IR::BaseType;
	switch (baseType) {
		case BaseType::FilesystemPath: {
			names.m_toCpp = "convertBaseFilesystemPathToCpp";
			names.m_toObjc = "convertBaseFilesystemPathToObjc";
			addNamespace(names, conversionNamespace);
			break;
		}
		case BaseType::String: {
			names.m_toCpp = "convertBaseStringToCpp";
			names.m_toObjc = "convertBaseStringToObjc";
			addNamespace(names, conversionNamespace);
			break;
		}
		case BaseType::StringView: {
			names.m_toCpp = "convertBaseStringViewToCpp";
			names.m_toObjc = "convertBaseStringViewToObjc";
			addNamespace(names, conversionNamespace);
			break;
		}

		case BaseType::Bool:
		case BaseType::Char16_t:
		case BaseType::Char32_t:
		case BaseType::Char:
		case BaseType::Complex:
		case BaseType::Double:
		case BaseType::Float:
		case BaseType::Int:
		case BaseType::LongDouble:
		case BaseType::LongInt:
		case BaseType::LongLongInt:
		case BaseType::ShortInt:
		case BaseType::SignedChar:
		case BaseType::UnsignedChar:
		case BaseType::UnsignedInt:
		case BaseType::UnsignedLongInt:
		case BaseType::UnsignedLongLongInt:
		case BaseType::UnsignedShortInt:
		case BaseType::Void:
		case BaseType::Wchar_t: return names;
	}

	return names;
}

Objc::Conversions::Conversion
getConversionContainerName(IR::Type const& type,
                           std::string const& conversionNamespace) {
	Conversion names;

	if (auto userDefined = std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		// Use the standard conversions for user defined types
		names = Objc::Conversions::getUserDefinedConversionNames(
		    userDefined->m_representation, "");
	} else {
		// Otherwise we build it ourself
		auto typeName = ObjcSwift::Helpers::buildTypeString(type);
		names.m_toCpp = fmt::format("convertContainer{}ToCpp", typeName);
		names.m_toObjc = fmt::format("convertContainer{}ToObjc", typeName);
	}

	return addNamespace(names, conversionNamespace);
}
}    // namespace Objc::Conversions
