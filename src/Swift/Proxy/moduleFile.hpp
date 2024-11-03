#pragma once

#include "Swift/Proxy/attribute.hpp"
#include "Swift/Proxy/class.hpp"
#include "Swift/Proxy/enum.hpp"
#include "Swift/Proxy/function.hpp"
#include "Swift/Proxy/module.hpp"
#include <filesystem>
#include <string>
#include <vector>

namespace Swift::Proxy {

class ModuleFile {
public:
	explicit ModuleFile(std::string const& libraryName);

	std::filesystem::path getFilepath() const;

	/**
	* The code that exposes the C++ code to Swift
	*/
	std::filesystem::path getSwiftFile() const;
	std::string getSwift() const;

	/**
	* Connects Objective-C++ to Swift
	*/
	std::filesystem::path getBridgingHeaderFile() const;
	std::string getBridgingHeader() const;

	void addModule(Module const& module);
	void addClass(Class const& cls);
	void addFunction(Function const& function);
	void addEnum(Enum const& e);
	void addAttribute(Attribute const& attribute);

private:
	std::string m_libraryName;

	enum class Structure {
		Module,
		Class,
		Function,
		Enum,
		Attribute,
	};

	std::string getSwift(Structure structure, size_t index) const;

	/**
	* NOTE: The order these are added is the order they will be put in the file
	*/
	void addStructure(Structure structure, size_t index);

	// Will be put one after each other
	std::vector<std::pair<Structure, size_t>> m_structures;

	std::vector<Module> m_modules;
	std::vector<Class> m_classes;
	std::vector<Function> m_functions;
	std::vector<Enum> m_enums;
	std::vector<Attribute> m_attributes;
};
}    // namespace Swift::Proxy
