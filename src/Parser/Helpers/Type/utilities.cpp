#include "Helpers/Type/utilities.hpp"
#include "Helpers/Type/Matchers/container.hpp"
#include "Helpers/Type/matchers.hpp"
#include <IR/ir.hpp>
#include <clang/AST/Type.h>
#include <optional>
#include <string_view>

namespace Helpers::Type {

clang::QualType getTypeWithPointersRemoved(clang::QualType type) {
	while (type->isAnyPointerType()) {
		type = type->getPointeeType();
	}
	return type;
}

/**
  * Take a value and build a type
  */
IR::Type buildTypeFromValue(IR::Type::Value value) {
	IR::Type type;
	type.m_type = value;
	return type;
}
/**
* Take a enum type and create a full type
*/
IR::Type buildTypeFromEnum(IR::Type::EnumValue enumValue) {
	IR::Type type;
	type.m_type = enumValue;
	return type;
}

/**
* Take a enum type and create a full type
*/
IR::Type buildTypeFromFunction(IR::Type::Function f) {
	IR::Type type;
	type.m_type = f;
	return type;
}

/**
* Take a user defined type and create a full type
*/
IR::Type buildTypeFromUserDefined(IR::Type::UserDefined userDefined) {
	IR::Type type;
	type.m_type = userDefined;
	return type;
}

/**
  * Take a container and build a type
  */
IR::Type buildTypeFromContainer(IR::ContainerType container) {
	IR::Type::Container c;
	c.m_container = container;

	IR::Type type;
	type.m_type = c;
	return type;
}

/**
* Tries to create an IR::Type (Int, Void, String, ...) of the input
* If it fails it returns std::nullopt
* NOTE: Assumes the qualifiers are removed (const, ...)
*/
std::optional<IR::Type> getIRType(std::string_view type) {
	if (auto value = getValueType(type)) {
		return buildTypeFromValue(value.value());
	} else if (auto containerType = Matchers::getContainerType(type)) {
		return buildTypeFromContainer(containerType.value());
	} else if (auto enumType = getEnumType(type)) {
		return buildTypeFromEnum(enumType.value());
	} else if (auto functionType = getFunctionMetaType(type)) {
		return buildTypeFromFunction(functionType.value());
	} else if (auto userDefined = getUserDefinedType(type)) {
		return buildTypeFromUserDefined(userDefined.value());
	}
	return {};
}

int getNumberOfPointers(clang::QualType type) {
	int numPointers = 0;
	while (type->isAnyPointerType()) {
		numPointers++;
		type = type->getPointeeType();
	}
	return numPointers;
}

bool isConst(clang::QualType const& type) {
	return type.split().Quals.hasConst();
}
}    // namespace Helpers::Type
