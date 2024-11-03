#pragma once

#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/cache.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <functional>
#include <queue>
#include <string>
#include <vector>

namespace Objc::Conversions::Container {
// Info needed for creating a set of conversion functions
// and registering it
struct ContainerData {
	explicit ContainerData(IR::Type const& _type,
	                       IR::ContainerType _containerType,
	                       std::vector<IR::Type> const& _containedTypes,
	                       std::vector<std::string>& _functions,
	                       Objc::Conversions::Conversion const& _names,
	                       std::string _noQualifiers);

	// The type to convert
	IR::Type const& type;
	IR::ContainerType containerType;
	std::vector<IR::Type> const& containedTypes;
	// Conversion functions to be registered
	std::vector<std::string>& functions;
	// Names of top level conversion functions
	Objc::Conversions::Conversion const& names;
	// The C++ type with no qualifiers
	std::string noQualifiers;
};

// Function called to create and register conversions
Objc::Conversions::Conversion
convertContainerWrapper(IR::Type const& type,
                        IR::ContainerType containerType,
                        std::vector<IR::Type> const& containedTypes,
                        std::vector<std::string>& functions,
                        Objc::Cache& cache,
                        std::function<void(ContainerData)> convertContainer);

std::string throwNSException(std::string const& condition,
                             std::string const& name,
                             std::string const& formatReason);

std::string getErrorCheck(ContainerData const& data);
}    // namespace Objc::Conversions::Container
