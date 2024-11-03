#pragma once

#include <string>
#include <vector>

namespace Swift {

struct SplitData {
	std::string m_name;
	std::string m_objcPrefix;
	std::string m_swiftPrefix;
};

SplitData splitIntoNames(std::string const& fullyQualifiedName,
                         std::string const& libraryName);

// E.g.
//   fullyQualifiedName = Long::Namespacy
//   libraryName = Very
//   Returns {"Very", "Very::Long", "Very::Long::Namespacy"}
std::vector<std::string>
splitIntoModules(std::string const& fullyQualifiedName);

}    // namespace Swift
