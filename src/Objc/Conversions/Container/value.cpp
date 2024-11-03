#include "Objc/Conversions/Container/value.hpp"
#include "Objc/Conversions/base.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {

void convertNSNumber(IR::Type const& type,
                     std::string const& typeName,
                     std::string const& numberWithName,
                     std::vector<std::string>& functions,
                     Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);
		functions.push_back(fmt::format(
		    R"(
{typeName} {toCppName}(id value) {{
  if (value && [value isKindOfClass:[NSNumber class]]) {{
    return [value {typeName}Value];
  }}
  return 0;
}})",
		    fmt::arg("toCppName", names.m_toCpp),
		    fmt::arg("typeName", typeName)));

		functions.push_back(fmt::format(
		    R"(
NSNumber* {toObjcName}({typeName} value) {{
  return [NSNumber numberWith{upperCaseType}:value];
}})",
		    fmt::arg("toObjcName", names.m_toObjc),
		    fmt::arg("typeName", typeName),
		    fmt::arg("upperCaseType", numberWithName)));
	}
}

void convertNSString(IR::Type const& type,
                     std::string const& typeName,
                     std::string const& getCString,
                     std::vector<std::string>& functions,
                     Objc::Cache& cache,
                     bool passByValue = false) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);

		functions.push_back(fmt::format(
		    R"(
{typeName} {toCppName}(NSString* s) {{
  return [s UTF8String];
}})",
		    fmt::arg("typeName", typeName),
		    fmt::arg("toCppName", names.m_toCpp)));

		functions.push_back(fmt::format(
		    R"(
NSString* {toObjcName}({typeName}{passByValue} s) {{
  return [[NSString alloc] initWithUTF8String:{getCString}];
}})",
		    fmt::arg("typeName", typeName),
		    fmt::arg("passByValue", passByValue ? "" : " const&"),
		    fmt::arg("getCString", getCString),
		    fmt::arg("toObjcName", names.m_toObjc)));
	}
}

void valueConversion(IR::Type const& type,
                     IR::BaseType baseType,
                     std::vector<std::string>& functions,
                     Objc::Cache& cache) {
	using IR::BaseType;
	switch (baseType) {
		case BaseType::Int: {
			convertNSNumber(type, "int", "Int", functions, cache);
			break;
		}
		case BaseType::Bool: {
			convertNSNumber(type, "bool", "Bool", functions, cache);
			break;
		}
		case BaseType::Char16_t: {
			break;
		}
		case BaseType::Char32_t: {
			break;
		}
		case BaseType::Char: {
			convertNSNumber(type, "char", "Char", functions, cache);
			break;
		}
		case BaseType::Complex: {
			break;
		}
		case BaseType::Double: {
			convertNSNumber(type, "double", "Double", functions, cache);
			break;
		}
		case BaseType::FilesystemPath: {
			convertNSString(type,
			                "std::filesystem::path",
			                "s.string().c_str()",
			                functions,
			                cache);
			break;
		}
		case BaseType::Float: {
			convertNSNumber(type, "float", "Float", functions, cache);
			break;
		}
		case BaseType::LongDouble: {
			break;
		}
		case BaseType::LongInt: {
			convertNSNumber(type, "long", "Long", functions, cache);
			break;
		}
		case BaseType::LongLongInt: {
			convertNSNumber(type, "long long", "LongLong", functions, cache);
			break;
		}
		case BaseType::ShortInt: {
			convertNSNumber(type, "short", "Short", functions, cache);
			break;
		}
		case BaseType::SignedChar: {
			convertNSNumber(type, "char", "Char", functions, cache);
			break;
		}
		case BaseType::String: {
			convertNSString(type, "std::string", "s.c_str()", functions, cache);
			break;
		}
		case BaseType::StringView: {
			convertNSString(type,
			                "std::string_view",
			                "std::string(s).c_str()",
			                functions,
			                cache,
			                true);
			break;
		}
		case BaseType::UnsignedChar: {
			convertNSNumber(
			    type, "unsigned char", "UnsignedChar", functions, cache);
			break;
		}
		case BaseType::UnsignedInt: {
			convertNSNumber(
			    type, "unsigned int", "UnsignedInt", functions, cache);
			break;
		}
		case BaseType::UnsignedLongInt: {
			convertNSNumber(
			    type, "unsigned long", "UnsignedLong", functions, cache);
			break;
		}
		case BaseType::UnsignedLongLongInt: {
			convertNSNumber(type,
			                "unsigned long long",
			                "UnsignedLongLong",
			                functions,
			                cache);
			break;
		}
		case BaseType::UnsignedShortInt: {
			convertNSNumber(
			    type, "unsigned short", "UnsignedShort", functions, cache);
			break;
		}
		case BaseType::Void: {
			break;
		}
		case BaseType::Wchar_t: {
			break;
		}
	}
}
}    // namespace Objc::Conversions::Container
