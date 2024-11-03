#include "Pybind/Proxy/class.hpp"
#include "Pybind/Helpers/getDocumentationParameter.hpp"
#include <fmt/format.h>
#include <string>

namespace Pybind::Proxy {

namespace {
std::string getInherited(std::vector<std::string> const& inheritedClasses) {
	std::string inherited =
	    fmt::format("{}", fmt::join(inheritedClasses, ", "));
	if (!inherited.empty()) {
		return fmt::format(", {}", inherited);
	}
	return inherited;
}
}    // namespace

std::string Class::getPybind(std::string const& moduleName) const {
	// Should this class be managed by a shared_ptr on the python side?
	std::string managedByShared =
	    m_isManagedByShared ?
	        fmt::format(", std::shared_ptr<{}>", m_fullyQualifiedName) :
            "";

	std::string out = fmt::format(
	    "\tpy::class_<{fullyQualifiedName}{managedByShared}{inherited}>({moduleName}, \"{name}\", {documentation})\n",
	    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
	    fmt::arg("managedByShared", managedByShared),
	    fmt::arg("inherited", getInherited(m_inherited)),
	    fmt::arg("name", m_name),
	    fmt::arg("documentation",
	             Pybind::Helpers::getDocumentationParameter(m_documentation)),
	    fmt::arg("moduleName", moduleName));

	for (auto const& init : m_constructors) {
		out += fmt::format("\t\t.{constructorPybind}\n",
		                   fmt::arg("constructorPybind", init.getPybind()));
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t\t.{functionPybind}\n",
		                   fmt::arg("functionPybind", function.getPybind()));
	}

	for (auto const& variable : m_memberVariables) {
		std::string accessor = variable.m_isConst ? "readonly" : "readwrite";
		std::string staticness = variable.m_isStatic ? "_static" : "";
		out += fmt::format(
		    "\t\t.def_{accessor}{staticness}(\"{variableName}\", &{fullyQualifiedClassName}::{variableName}, {documentation})\n",
		    fmt::arg("accessor", accessor),
		    fmt::arg("staticness", staticness),
		    fmt::arg("fullyQualifiedClassName", m_fullyQualifiedName),
		    fmt::arg("variableName", variable.m_name),
		    fmt::arg("documentation",
		             Pybind::Helpers::getDocumentationParameter(
		                 variable.m_documentation)));
	}

	// The last newline
	out.pop_back();
	out.push_back(';');
	out.push_back('\n');

	// To put the enums at the end of the class
	if (!m_enums.empty()) {
		out.push_back('\n');

		for (auto const& e : m_enums) {
			out += fmt::format("{}\n", e.getPybind(m_name));
		}
		// The last endline
		out.pop_back();
		// The last ';'
		out.pop_back();
	}

	return out;
}

Class::Class(std::string const& name, std::string const& fullyQualifiedName)
    : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_constructors(),
      m_functions(), m_memberVariables(), m_enums(),
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

void Class::addMemberVariable(std::string const& variableName,
                              std::string const& documentation,
                              bool isConst,
                              bool isStatic) {
	m_memberVariables.push_back(
	    {variableName, documentation, isConst, isStatic});
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

void Class::addTrampolineClass(std::string const& trampolineClass) {
	m_inherited.push_back(trampolineClass);
}
}    // namespace Pybind::Proxy
