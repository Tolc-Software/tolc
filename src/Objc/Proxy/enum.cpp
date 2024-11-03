#include "Objc/Proxy/enum.hpp"
#include "Objc/utility.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Objc::Proxy {

Enum::Enum(std::string const& name)
    : m_name(name), m_documentation(), m_values({}), m_isScoped(false) {};

std::string Enum::getObjcSource() const {
	return "";
}

std::string Enum::getObjcHeader() const {
	return fmt::format(R"({documentation}
typedef NS_ENUM(int, {name}) {{ {values} }};
)",
	                   fmt::arg("documentation",
	                            Objc::getDocumentationString(m_documentation)),
	                   fmt::arg("name", m_name),
	                   fmt::arg("values", fmt::join(m_values, ", ")));
}

void Enum::setScoped(bool isScoped) {
	m_isScoped = isScoped;
}

void Enum::addValue(std::string const& value) {
	m_values.push_back(value);
}

void Enum::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

std::string Enum::getName() const {
	return m_name;
}

std::vector<std::string> Enum::getValues() const {
	return m_values;
}

}    // namespace Objc::Proxy
