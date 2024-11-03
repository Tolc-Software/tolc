#pragma once

#include <set>
#include <string>

namespace Swift {

struct Cache {
	// All the defined symbols (namespaces and classes)
	std::set<std::string> m_definedSymbols;

	// The name of the users module we are converting
	std::string m_moduleName;
};

}    // namespace Swift
