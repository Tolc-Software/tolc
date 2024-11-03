#include "Swift/Builders/moduleBuilder.hpp"
#include "Swift/Proxy/module.hpp"
#include "Swift/cache.hpp"
#include "Swift/getName.hpp"
#include <string>
#include <vector>

namespace Swift::Builders {

std::vector<Swift::Proxy::Module>
buildModules(Objc::Proxy::Class const& objcClass, Swift::Cache& cache) {
	std::vector<Swift::Proxy::Module> modules;

	// Make sure that we create the potentially empty modules
	// My::Namespacy::func()
	// Requires both My and My.Namespacy to be defined
	for (auto const& module :
	     Swift::splitIntoModules(objcClass.getCppClassName())) {
		if (!cache.m_definedSymbols.contains(module)) {
			auto splitData = Swift::splitIntoNames(module, cache.m_moduleName);
			modules.emplace_back(splitData.m_name, splitData.m_swiftPrefix);

			cache.m_definedSymbols.insert(module);
		}
	}

	return modules;
}
}    // namespace Swift::Builders
