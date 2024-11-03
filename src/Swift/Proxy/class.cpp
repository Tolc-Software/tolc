#include "Swift/Proxy/class.hpp"
#include "ObjcSwift/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

namespace {
std::string
joinFunctions(std::vector<Swift::Proxy::Function> const& functions) {
	std::string out;
	for (auto const& f : functions) {
		out += f.getSwift() + '\n';
	}
	return out;
}
}    // namespace

std::string Class::getSwift() const {
	std::string out = fmt::format(
	    R"(
extension {extending} {{
  public class {className} {{
    private var m_object: {objcClassName}

{constructors}{functions}{memberVariables}
  }}
}}
)",
	    fmt::arg("extending", m_extending),
	    fmt::arg("className", m_name),
	    fmt::arg("objcClassName", m_objcClassName),
	    fmt::arg("constructors", joinFunctions(m_constructors)),
	    fmt::arg("functions", joinFunctions(m_functions)),
	    fmt::arg("memberVariables", joinMemberVariables()));

	return out;
}

Class::Class(std::string const& name,
             std::string const& objcClassName,
             std::string const& extending)
    : m_name(name), m_objcClassName(objcClassName), m_extending(extending),
      m_constructors(), m_functions(), m_memberVariables(), m_enums(),
      m_isManagedByShared(false) {}

void Class::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Class::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Class::addConstructor(Function const& constructor) {
	m_constructors.push_back(constructor);
}

void Class::addAttribute(Attribute const& attribute) {
	m_memberVariables.push_back(attribute);
}

std::string const& Class::getName() const {
	return m_name;
}

void Class::setAsManagedByShared() {
	m_isManagedByShared = true;
}

void Class::setDocumentation(std::string const& documentation) {
	m_documentation = documentation;
}

void Class::setInherited(std::vector<std::string> const& inherited) {
	for (auto const& i : inherited) {
		m_inherited.push_back(i);
	}
}
std::string Class::joinMemberVariables() const {
	std::string out;
	for (auto const& m : m_memberVariables) {
		out += m.getSwift();
	}
	return out;
}

}    // namespace Swift::Proxy
