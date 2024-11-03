#include "Pybind/checkType.hpp"
#include "Pybind/Helpers/types.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <queue>
#include <set>
#include <spdlog/spdlog.h>
#include <string>
#include <variant>

namespace Pybind {

std::optional<std::string> getInclude(IR::ContainerType container) {
	// See https://pybind11.readthedocs.io/en/stable/advanced/cast/overview.html
	switch (container) {
		case IR::ContainerType::Array:
		case IR::ContainerType::Deque:
		case IR::ContainerType::List:
		case IR::ContainerType::Map:
		case IR::ContainerType::Optional:
		case IR::ContainerType::Set:
		case IR::ContainerType::UnorderedMap:
		case IR::ContainerType::UnorderedSet:
		case IR::ContainerType::Valarray:
		case IR::ContainerType::Variant:
		case IR::ContainerType::Vector: return "<pybind11/stl.h>"; break;

		case IR::ContainerType::ForwardList:
		case IR::ContainerType::MultiMap:
		case IR::ContainerType::MultiSet:
		case IR::ContainerType::PriorityQueue:
		case IR::ContainerType::Queue:
		case IR::ContainerType::Stack:
		case IR::ContainerType::UnorderedMultiMap:
		case IR::ContainerType::UnorderedMultiSet:
			spdlog::error(
			    "Container type {} does not currently have a direct translation via pybind11. The translation might not work.",
			    Pybind::Helpers::toString(container));
			break;

		case IR::ContainerType::Allocator:
		case IR::ContainerType::EqualTo:
		case IR::ContainerType::Greater:
		case IR::ContainerType::Hash:
		case IR::ContainerType::Less:
		case IR::ContainerType::Pair:
		case IR::ContainerType::SharedPtr:
		case IR::ContainerType::Tuple:
		case IR::ContainerType::UniquePtr: break;
	}
	return std::nullopt;
}

/**
* Go through type and check if it requires any extra pybind11 includes
* E.g. vector conversion requires inclusion of <pybind11/stl.h>
*/
std::optional<std::string> extractInclude(IR::Type const& type) {
	if (auto container = Pybind::Helpers::getContainer(type)) {
		return getInclude(container->m_container);
	} else if (Pybind::Helpers::isFunctionType(type)) {
		// The type is std::function
		return "<pybind11/functional.h>";
	} else if (Pybind::Helpers::isBaseType(type,
	                                       IR::BaseType::FilesystemPath)) {
		// The type is std::filesystem::path
		return "<pybind11/stl/filesystem.h>";
	} else if (Pybind::Helpers::isBaseType(type, IR::BaseType::Complex)) {
		// The type is std::complex
		return "<pybind11/complex.h>";
	}
	return std::nullopt;
}

bool isSharedPtr(IR::ContainerType c) {
	return c == IR::ContainerType::SharedPtr;
}

std::optional<std::string>
getFirstUserDefinedRepresentation(std::vector<IR::Type> const& types) {
	if (!types.empty()) {
		if (auto userDefined = Pybind::Helpers::getUserDefined(types[0])) {
			return userDefined->m_representation;
		}
	}
	return std::nullopt;
}

std::optional<std::string> extractShared(IR::Type const& type) {
	if (auto container = Pybind::Helpers::getContainer(type)) {
		if (isSharedPtr(container->m_container)) {
			return getFirstUserDefinedRepresentation(
			    container->m_containedTypes);
		}
	}
	return std::nullopt;
}

void checkType(IR::Type const& type, Pybind::Proxy::TypeInfo& info) {
	std::queue<std::reference_wrapper<IR::Type const>> typesToCheck;
	typesToCheck.push(type);

	while (!typesToCheck.empty()) {
		auto& current = typesToCheck.front();

		if (auto include = extractInclude(current)) {
			info.m_includes.insert(fmt::format("#include {}", include.value()));
		}

		if (auto sharedClass = extractShared(current)) {
			info.m_classesMarkedShared.insert(sharedClass.value());
		}

		if (auto container = Pybind::Helpers::getContainer(current)) {
			for (auto const& containedType : container->m_containedTypes) {
				typesToCheck.push(containedType);
			}
		}
		typesToCheck.pop();
	}
}

}    // namespace Pybind
