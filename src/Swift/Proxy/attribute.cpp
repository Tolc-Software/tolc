#include "Swift/Proxy/attribute.hpp"
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace Swift::Proxy {

namespace {
std::string getGetter(std::string_view body) {
	if (body.empty()) {
		return "";
	}
	return fmt::format(R"(
    get {{
      return {}
    }})",
	                   body);
}

std::string getSetter(std::string_view body) {
	if (body.empty()) {
		return "";
	}
	return fmt::format(R"(
    set(newValue) {{
      return {} = newValue
    }})",
	                   body);
}
}    // namespace

Attribute::Attribute(std::string const& name, std::string const& typeName)
    : m_name(name), m_typeName(typeName), m_getter(), m_setter(),
      m_isStatic(false) {}

std::string Attribute::getSwift() const {
	return fmt::format(R"(
  public {static}var {name}: {typeName} {{{getter}{setter}
  }})",
	                   fmt::arg("static", m_isStatic ? "static " : ""),
	                   fmt::arg("name", m_name),
	                   fmt::arg("typeName", m_typeName),
	                   fmt::arg("getter", getGetter(m_getter)),
	                   fmt::arg("setter", getSetter(m_setter)));
}

void Attribute::addGetter(std::string const& body) {
	m_getter = body;
}

void Attribute::addSetter(std::string const& body) {
	m_setter = body;
}

void Attribute::setAsStatic() {
	m_isStatic = true;
}

}    // namespace Swift::Proxy
