#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

namespace Objc {

struct Cache {
	std::string m_extraFunctionsNamespace = "Tolc_";

	// Functions that are needed globally from Objective-C
	// Will be placed in the root under the namespace m_extraFunctionsNamespace
	std::vector<std::string> m_extraFunctions;

	// Conversion functions for user defined types
	std::vector<std::string> m_extraClassConversions;

	// Classes that are used in a shared_ptr
	// Set as fully qualified name of the class
	std::set<std::string> m_sharedPtrClasses;

	// {C++ fully qualified name, Objective-C fully qualified name}
	std::map<std::string, std::string> m_cppToObjcClassNames;

	// The name of the users module we are converting
	std::string m_moduleName;

	// Registered conversion names
	std::set<std::string> m_conversions;
};

}    // namespace Objc
