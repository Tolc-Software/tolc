#include "Objc/Conversions/container.hpp"
#include "Objc/Conversions//Container/container.hpp"
#include "Objc/Conversions/Container/value.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <queue>
#include <string>
#include <vector>

namespace Objc::Conversions {

Conversion getContainerTypeConversions(IR::Type const& type,
                                       IR::Type::Container const& container,
                                       Objc::Cache& cache) {
	// Types that need conversion
	std::queue<IR::Type const*> typesToConvert;
	// Conversion functions that has been defined so far
	// Since vector<int> will first convert vector, then int
	// We need to lay these out in the file in the reverse order
	// so that convertVector can call convertInt
	std::vector<std::string> extraFunctions;

	auto rootConversion = Container::containerConversion(
	    type, container, extraFunctions, typesToConvert, cache);

	while (!typesToConvert.empty()) {
		auto* currentType = typesToConvert.front();
		if (auto currentContainer =
		        std::get_if<IR::Type::Container>(&currentType->m_type)) {
			Container::containerConversion(*currentType,
			                               *currentContainer,
			                               extraFunctions,
			                               typesToConvert,
			                               cache);
		} else if (auto value =
		               std::get_if<IR::Type::Value>(&currentType->m_type)) {
			Container::valueConversion(
			    *currentType, value->m_base, extraFunctions, cache);
		}
		typesToConvert.pop();
	}

	cache.m_extraFunctions.insert(cache.m_extraFunctions.end(),
	                              extraFunctions.rbegin(),
	                              extraFunctions.rend());
	return rootConversion;
}
}    // namespace Objc::Conversions

