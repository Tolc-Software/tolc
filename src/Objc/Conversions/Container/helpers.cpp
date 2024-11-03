#include "Objc/Conversions/Container/helpers.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/utility.hpp"
#include "Objc/cache.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <functional>
#include <queue>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {
// Info needed for creating a set of conversion functions
// and registering it
ContainerData::ContainerData(IR::Type const& _type,
                             IR::ContainerType _containerType,
                             std::vector<IR::Type> const& _containedTypes,
                             std::vector<std::string>& _functions,
                             Objc::Conversions::Conversion const& _names,
                             std::string _noQualifiers)
    : type(_type), containerType(_containerType),
      containedTypes(_containedTypes), functions(_functions), names(_names),
      noQualifiers(_noQualifiers) {}

// Function called to create and register conversions
Objc::Conversions::Conversion
convertContainerWrapper(IR::Type const& type,
                        IR::ContainerType containerType,
                        std::vector<IR::Type> const& containedTypes,
                        std::vector<std::string>& functions,
                        Objc::Cache& cache,
                        std::function<void(ContainerData)> convertContainer) {
	auto names = Objc::Conversions::getConversionContainerName(type);
	if (!cache.m_conversions.contains(names.m_toCpp)) {
		cache.m_conversions.insert(names.m_toCpp);
		// TODO: Handle error
		if (!containedTypes.empty()) {
			ContainerData d(type,
			                containerType,
			                containedTypes,
			                functions,
			                names,
			                ObjcSwift::Helpers::removeQualifiers(type));
			convertContainer(d);
		}
	}
	return addNamespace(names, cache.m_extraFunctionsNamespace);
}

std::string throwNSException(std::string const& condition,
                             std::string const& name,
                             std::string const& formatReason) {
	return fmt::format(R"(
  if ({condition}) {{
    @throw [NSException
      exceptionWithName:@"{name}"
      reason:[NSString
        stringWithFormat:{formatReason}]
      userInfo:nil];
  }})",
	                   fmt::arg("condition", condition),
	                   fmt::arg("name", name),
	                   fmt::arg("formatReason", formatReason));
}

std::string getErrorCheck(ContainerData const& data) {
	using IR::ContainerType;
	switch (data.containerType) {
		case ContainerType::Pair: {
			return throwNSException(
			    "[p count] != 2",
			    "TypeException",
			    R"(@"The array passed does not match the number of types in a pair. Expected: 2, Got: %lu.", [p count])");
		}
		case ContainerType::Tuple: {
			std::string tupleSize = std::to_string(data.containedTypes.size());
			return throwNSException(
			    "[t count] != " + tupleSize,
			    "TypeException",
			    fmt::format(
			        R"(@"The array passed does not match the number of types expected in the tuple. Expected: {}, Got: %lu.", [t count])",
			        tupleSize));
		}
		case ContainerType::Array: {
			std::string arraySize = data.containedTypes.back().m_representation;
			return throwNSException(
			    "[v count] != " + arraySize,
			    "TypeException",
			    fmt::format(
			        R"(@"The size of the array does not match the expected fixed size. Expected: {}, Got: %lu.", [v count])",
			        arraySize));
		}
		default: break;
	}
	return "";
}
}    // namespace Objc::Conversions::Container
