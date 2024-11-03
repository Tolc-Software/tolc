#include "Objc/Conversions/base.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <array>
#include <fmt/format.h>
#include <string>
#include <vector>

namespace Objc::Conversions {

namespace {

Conversion convertString(Objc::Cache& cache) {
	auto names = Objc::Conversions::getConversionBaseName(IR::BaseType::String);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
std::string {toCppName}(NSString* s) {{
  return [s UTF8String];
}})",
		    fmt::arg("toCppName", names.m_toCpp)));

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
NSString* {toObjcName}(std::string const& s) {{
  return [[NSString alloc] initWithUTF8String:s.c_str()];
}})",
		    fmt::arg("toObjcName", names.m_toObjc)));
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

Conversion convertStringView(Objc::Cache& cache) {
	auto names =
	    Objc::Conversions::getConversionBaseName(IR::BaseType::StringView);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
std::string {toCppName}(NSString* s) {{
  return [s UTF8String];
}})",
		    fmt::arg("toCppName", names.m_toCpp)));

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
NSString* {toObjcName}(std::string_view s) {{
  NSData *data = [NSData dataWithBytes:s.data() length:s.length()];
  return [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
}})",
		    fmt::arg("toObjcName", names.m_toObjc)));
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

Conversion convertFileSystem(Objc::Cache& cache) {
	auto names =
	    Objc::Conversions::getConversionBaseName(IR::BaseType::FilesystemPath);

	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
std::filesystem::path {toCppName}(NSString* s) {{
  return [s UTF8String];
}})",
		    fmt::arg("toCppName", names.m_toCpp)));

		cache.m_extraFunctions.push_back(fmt::format(
		    R"(
NSString* {toObjcName}(std::filesystem::path const& p) {{
  return [[NSString alloc] initWithUTF8String:p.c_str()];
}})",
		    fmt::arg("toObjcName", names.m_toObjc)));
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}
}    // namespace

Conversion getBaseTypeConversions(IR::BaseType baseType, Objc::Cache& cache) {
	Conversion c;
	c.m_toCpp = "";
	c.m_toObjc = "";
	using IR::BaseType;
	switch (baseType) {
		case BaseType::FilesystemPath: return convertFileSystem(cache);
		case BaseType::String: return convertString(cache);
		case BaseType::StringView: return convertStringView(cache);

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
		case BaseType::Wchar_t: return c;
	}
	return c;
}

}    // namespace Objc::Conversions
