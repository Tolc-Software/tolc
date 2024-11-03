#include "ObjcSwift/Helpers/types.hpp"
#include "ObjcSwift/Helpers/string.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <variant>

namespace ObjcSwift::Helpers {
IR::Type::Container const* getContainer(IR::Type const& type) {
	if (auto container = std::get_if<IR::Type::Container>(&type.m_type)) {
		return container;
	}
	return nullptr;
}

IR::Type::UserDefined const* getUserDefined(IR::Type const& type) {
	if (auto userDefined = std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		return userDefined;
	}
	return nullptr;
}

bool isBaseType(IR::Type const& type, IR::BaseType base) {
	if (auto value = std::get_if<IR::Type::Value>(&type.m_type)) {
		return value->m_base == base;
	}
	return false;
}

bool isContainerType(IR::Type const& type, IR::ContainerType container) {
	if (auto c = ObjcSwift::Helpers::getContainer(type)) {
		return c->m_container == container;
	}
	return false;
}

bool isFunctionType(IR::Type const& type) {
	return std::holds_alternative<IR::Type::Function>(type.m_type);
}

std::pair<std::string, std::string> removeCppTemplate(std::string name) {
	// MyClass<int> -> {MyClass, <int>}
	auto templateStart = name.find('<');
	if (templateStart == std::string::npos) {
		// No template
		return {name, ""};
	}
	auto withoutTemplate = name.substr(0, templateStart);
	name.erase(0, templateStart);
	return {withoutTemplate, name};
}

std::string removeQualifiers(IR::Type const& type) {
	auto noQuals = type.m_representation;
	if (type.m_isConst) {
		noQuals = removeSubString(noQuals, "const ");
	}
	if (type.m_isReference) {
		noQuals = removeSubString(noQuals, "&", true);
	}
	for (int i = 0; i < type.m_numPointers; ++i) {
		noQuals = removeSubString(noQuals, "*", true);
	}
	return noQuals;
}

}    // namespace ObjcSwift::Helpers
