#include "Pybind/Proxy/enum.hpp"
#include "Pybind/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Pybind::Proxy {

Enum::Enum(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_values({}),
      m_isScoped(false) {};

std::string Enum::getPybind(std::string const& moduleOrClass) const {
	std::string out = fmt::format(
	    "\t\tpy::enum_<{fullyQualifiedName}>({moduleOrClass}, \"{name}\"",
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	    fmt::arg("name", m_name),
	    fmt::arg("moduleOrClass", moduleOrClass));

	if (m_isScoped) {
		out += ", py::arithmetic()";
	}
	out += fmt::format(
	    ", {})\n", Pybind::Helpers::getDocumentationParameter(m_documentation));

	for (auto const& value : m_values) {
		out += fmt::format(
		    "\t\t.value(\"{value}\", {fullyQualifiedName}::{value})\n",
		    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
		    fmt::arg("value", value));
	}

	if (!m_isScoped) {
		out += "\t\t.export_values()\n";
	}
	// Remove the last newline
	out.pop_back();
	out += ";\n";

	return out;
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

}    // namespace Pybind::Proxy
