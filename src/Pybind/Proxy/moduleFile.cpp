#include "Pybind/Proxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace Pybind::Proxy {

ModuleFile::ModuleFile(Module const& rootModule, std::string const& libraryName)
    : m_rootModuleName(rootModule.getVariableName()),
      m_libraryName(libraryName), m_modules({rootModule}), m_typeInfo() {}

void ModuleFile::addModule(Module const& m) {
	m_modules.push_back(m);
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_libraryName + "_python.cpp";
}

void ModuleFile::setTypeInfo(Pybind::Proxy::TypeInfo const& info) {
	m_typeInfo = info;
	m_typeInfo.m_includes.insert("#include <pybind11/pybind11.h>");
}

std::string getExtraFunctions(Pybind::Proxy::TypeInfo const& typeInfo) {
	if (typeInfo.m_trampolineClasses.empty()) {
		return "";
	}
	return fmt::format(
	    R"_tolc_delimiter(
namespace {ns} {{
{joinedClasses}
}}
)_tolc_delimiter",
	    fmt::arg("ns", typeInfo.m_extraFunctionsNamespace),
	    fmt::arg("joinedClasses",
	             fmt::join(typeInfo.m_trampolineClasses, "\n")));
}

std::string ModuleFile::getPybind() const {
	std::string out = fmt::format(
	    R"(
{includes}

namespace py = pybind11;
{extraFunctions}
PYBIND11_MODULE({libraryName}, {rootModuleName}))",
	    fmt::arg("includes", fmt::join(m_typeInfo.m_includes, "\n")),
	    fmt::arg("extraFunctions", getExtraFunctions(m_typeInfo)),
	    fmt::arg("libraryName", m_libraryName),
	    fmt::arg("rootModuleName", m_rootModuleName));

	out += " {\n";
	for (auto const& m : m_modules) {
		out += m.getPybind();
	}
	out += '}';

	return out;
}
}    // namespace Pybind::Proxy
