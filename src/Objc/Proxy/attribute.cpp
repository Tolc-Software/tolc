#include "Objc/Proxy/attribute.hpp"
#include "Objc/utility.hpp"
#include "ObjcSwift/Helpers/wrapInFunction.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

Attribute::Attribute(std::string const& cppClass,
                     std::string const& objcClass,
                     std::string const& name)
    : m_name(name), m_cppClass(cppClass), m_objcClass(objcClass), m_type(),
      m_documentation(), m_isConst(false), m_isStatic(false), m_isObject(false),
      m_isStandalone(false) {}

std::string Attribute::getObjcSource() const {
	std::string out;
	std::string staticAttribute = "-";
	std::string callAccess = "m_object->";
	if (m_isStatic) {
		staticAttribute = "+";
		callAccess = m_cppClass + "::";
	}

	// Maybe need a conversion to Objective-C type
	auto call = ObjcSwift::Helpers::wrapInFunction(
	    callAccess + m_name, m_type.m_conversions.m_toObjc);
	out += fmt::format(R"(
{staticAttribute}({type}) {name} {{
  return {call};
}}
)",
	                   fmt::arg("staticAttribute", staticAttribute),
	                   fmt::arg("type", m_type.m_name()),
	                   fmt::arg("name", m_name),
	                   fmt::arg("call", call));
	if (!m_isConst) {
		// Not const -> Add a setter
		// Maybe need a conversion to C++ type
		auto convertedVariable = ObjcSwift::Helpers::wrapInFunction(
		    "new" + m_name, m_type.m_conversions.m_toCpp);
		out += fmt::format(
		    R"(
{staticAttribute}(void) {name}:({type})new{name} {{
  {callAccess}{name} = {convertedVariable};
}}
)",
		    fmt::arg("staticAttribute", staticAttribute),
		    fmt::arg("name", m_name),
		    fmt::arg("type", m_type.m_name()),
		    fmt::arg("callAccess", callAccess),
		    fmt::arg("convertedVariable", convertedVariable));
	}

	if (m_isStandalone) {
		return Objc::wrapInImplementation(
		    m_objcClass, Objc::getCategoryName('a', m_objcClass, m_name), out);
	}
	return out;
}

std::string Attribute::getObjcHeader() const {
	auto declaration = fmt::format("@property {options} {type} {name};\n",
	                               fmt::arg("options", getPropertyOptions()),
	                               fmt::arg("type", m_type.m_name()),
	                               fmt::arg("name", m_name));
	if (m_isStandalone) {
		declaration = Objc::wrapInInterface(
		    m_objcClass,
		    Objc::getCategoryName('a', m_objcClass, m_name),
		    declaration);
	}

	return fmt::format(R"({documentation}
{declaration}
)",
	                   fmt::arg("documentation",
	                            Objc::getDocumentationString(m_documentation)),
	                   fmt::arg("declaration", declaration));
}

std::string Attribute::getPropertyOptions() const {
	std::vector<std::string> options;
	if (m_isObject) {
		// Memory management
		options.push_back("strong");
	}

	if (m_isConst) {
		options.push_back("readonly");
	} else {
		// Add a setter
		options.push_back(fmt::format("setter={}:", m_name));
	}

	if (m_isStatic) {
		options.push_back("class");
	}

	switch (m_type.m_nullability) {
		case Proxy::Nullability::Nonnull: options.push_back("nonnull"); break;
		case Proxy::Nullability::Nullable: options.push_back("nullable"); break;
		case Proxy::Nullability::NotApplicable: break;
	}

	return fmt::format("({})", fmt::join(options, ", "));
}

void Attribute::setType(Objc::Proxy::Type const& type) {
	m_type = type;
}

void Attribute::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

void Attribute::setAsConst() {
	m_isConst = true;
}

void Attribute::setAsStatic() {
	m_isStatic = true;
}

bool Attribute::isStatic() const {
	return m_isStatic;
}

bool Attribute::isConst() const {
	return m_isConst;
}

void Attribute::setAsObject() {
	m_isObject = true;
}

void Attribute::setAsStandalone() {
	m_isStandalone = true;
}

bool Attribute::isStandalone() const {
	return m_isStandalone;
}

Objc::Proxy::Type const& Attribute::getType() const {
	return m_type;
}

std::string const& Attribute::getName() const {
	return m_name;
}
}    // namespace Objc::Proxy
