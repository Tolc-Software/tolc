#include "Swift/Proxy/moduleFile.hpp"
#include <fmt/format.h>
#include <string>

namespace Swift::Proxy {

ModuleFile::ModuleFile(std::string const& libraryName)
    : m_libraryName(libraryName) {}

void ModuleFile::addStructure(Structure structure, size_t index) {
	m_structures.push_back({structure, index});
}

std::string ModuleFile::getSwift(Structure structure, size_t index) const {
	switch (structure) {
		case Structure::Module: {
			return m_modules[index].getSwift();
		}
		case Structure::Class: {
			return m_classes[index].getSwift();
		}
		case Structure::Function: {
			return m_functions[index].getSwift();
		}
		case Structure::Enum: {
			return m_enums[index].getSwift();
		}
		case Structure::Attribute: {
			return m_attributes[index].getSwift();
		}
	}
	// Should never happen
	return "// Something went wrong";
}

std::filesystem::path ModuleFile::getFilepath() const {
	return m_libraryName + "_swift.cpp";
}

std::string ModuleFile::getSwift() const {
	std::string swift = fmt::format(R"(public class {libraryName} {{}}

)",
	                                fmt::arg("libraryName", m_libraryName));

	for (auto [structure, index] : m_structures) {
		swift += getSwift(structure, index);
	}

	return swift;
}

std::filesystem::path ModuleFile::getSwiftFile() const {
	return m_libraryName + "_swift.swift";
}

void ModuleFile::addModule(Module const& module) {
	m_modules.push_back(module);
	addStructure(Structure::Module, m_modules.size() - 1);
}

void ModuleFile::addClass(Class const& cls) {
	m_classes.push_back(cls);
	addStructure(Structure::Class, m_classes.size() - 1);
}

void ModuleFile::addFunction(Function const& function) {
	m_functions.push_back(function);
	addStructure(Structure::Function, m_functions.size() - 1);
}

void ModuleFile::addEnum(Enum const& e) {
	m_enums.push_back(e);
	addStructure(Structure::Enum, m_enums.size() - 1);
}

void ModuleFile::addAttribute(Attribute const& attribute) {
	m_attributes.push_back(attribute);
	addStructure(Structure::Attribute, m_attributes.size() - 1);
}

}    // namespace Swift::Proxy
