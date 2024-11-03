#include "Objc/Conversions/userDefined.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace Objc::Conversions {

namespace {
std::string getValidFunctionName(std::string_view name) {
	std::string out;
	for (size_t i = 0; i < name.size(); ++i) {
		switch (name[i]) {
			case '<':
			case '>':
			case ' ':
			case ':':
			case ',': out.push_back('_'); break;
			default: out.push_back(name[i]);
		}
	}
	return out;
}

}    // namespace
Objc::Conversions::Conversion
getUserDefinedConversionNames(std::string_view fullyQualifiedName,
                              std::string_view ns) {
	Objc::Conversions::Conversion names;
	auto validName = getValidFunctionName(fullyQualifiedName);
	names.m_toObjc = fmt::format("convertUserDefined{}ToObjc", validName);
	names.m_toCpp = fmt::format("convertUserDefined{}ToCpp", validName);
	return Objc::Conversions::addNamespace(names, std::string(ns));
}
}    // namespace Objc::Conversions
