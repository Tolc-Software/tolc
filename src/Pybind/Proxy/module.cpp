#include "Pybind/Proxy/module.hpp"
#include "Pybind/Helpers/getDocumentationParameter.hpp"
#include <algorithm>
#include <fmt/format.h>
#include <string>

namespace Pybind::Proxy {

std::string Module::getPybind() const {
	std::string out;
	for (auto const& cls : m_classes) {
		out += fmt::format("{}\n", cls.getPybind(m_variableName));
	}

	for (auto const& e : m_enums) {
		out += fmt::format("{}\n", e.getPybind(m_variableName));
	}

	for (auto const& attribute : m_attributes) {
		out += fmt::format("{}.{}\n", m_variableName, attribute.getPybind());
	}

	for (auto const& function : m_functions) {
		out += fmt::format("\t{}.{};\n", m_variableName, function.getPybind());
	}

	// Define all the children
	for (auto const& [submoduleName, submoduleVariable, documentation] :
	     m_submodules) {
		out += fmt::format(
		    "\tauto {submoduleVariable} = {variableName}.def_submodule(\"{submoduleName}\", {docs});\n",
		    fmt::arg("submoduleVariable", submoduleVariable),
		    fmt::arg("submoduleName", submoduleName),
		    fmt::arg("variableName", m_variableName),
		    fmt::arg(
		        "docs",
		        Pybind::Helpers::getDocumentationParameter(documentation)));
	}

	return out;
}

Module::Module(std::string const& variableName)
    : m_variableName(variableName), m_submodules({}), m_functions(), m_enums(),
      m_attributes() {}

void Module::addFunction(Function const& function) {
	m_functions.push_back(function);
}

void Module::addClass(Class const& c) {
	m_classes.push_back(c);
}

void Module::addEnum(Enum const& e) {
	m_enums.push_back(e);
}

void Module::addAttribute(Attribute const& a) {
	m_attributes.push_back(a);
}

std::string const& Module::getVariableName() const {
	return m_variableName;
}

void Module::addSubmodule(std::string const& name,
                          std::string const& variableName,
                          std::string const& documentation) {
	m_submodules.push_back({name, variableName, documentation});
}

}    // namespace Pybind::Proxy
