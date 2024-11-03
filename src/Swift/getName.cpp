#include "Swift/getName.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include <fmt/format.h>
#include <numeric>
#include <string>
#include <vector>

namespace Swift {

std::vector<std::string>
splitIntoModules(std::string const& fullyQualifiedName) {
	std::vector<std::string> modules;

	auto splitted = ObjcSwift::Helpers::split(fullyQualifiedName, "::");

	// E.g.
	//   splitted = {"Very", "Long", "Namespacy"}
	//  = modules = {"Very", "Very::Long", "Very::Long::Namespacy"}
	std::partial_sum(
	    splitted.begin(),
	    splitted.end(),
	    std::back_inserter(modules),
	    [](std::string const& soFar, std::string const& nextNamespace) {
		    return soFar + "::" + nextNamespace;
	    });

	return modules;
}

SplitData splitIntoNames(std::string const& fullyQualifiedName,
                         std::string const& libraryName) {
	SplitData data;

	auto splitted = ObjcSwift::Helpers::split(fullyQualifiedName, "::");
	if (splitted.size() == 1) {
		data.m_name = splitted[0];
		data.m_objcPrefix = "_" + libraryName;
		data.m_swiftPrefix = libraryName;
	} else {
		data.m_name = splitted.back();
		// Remove the name
		splitted.pop_back();
		splitted.push_front(libraryName);
		data.m_objcPrefix = fmt::format("_{}", fmt::join(splitted, ""));
		data.m_swiftPrefix = fmt::format("{}", fmt::join(splitted, "."));
	}

	return data;
}

}    // namespace Swift

