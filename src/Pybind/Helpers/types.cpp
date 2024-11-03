#include "Pybind/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <variant>

namespace Pybind::Helpers {
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
	if (auto c = Pybind::Helpers::getContainer(type)) {
		return c->m_container == container;
	}
	return false;
}

bool isFunctionType(IR::Type const& type) {
	return std::holds_alternative<IR::Type::Function>(type.m_type);
}

std::string removeCppTemplate(std::string const& name) {
	// MyClass<int> -> MyClass
	return name.substr(0, name.find('<'));
}

std::string toString(IR::ContainerType c) {
	switch (c) {
		case IR::ContainerType::Allocator: return "std::allocator";
		case IR::ContainerType::Array: return "std::array";
		case IR::ContainerType::Deque: return "std::deque";
		case IR::ContainerType::EqualTo: return "std::equal_to";
		case IR::ContainerType::ForwardList: return "std::forward_list";
		case IR::ContainerType::Greater: return "std::greater";
		case IR::ContainerType::Hash: return "std::hash";
		case IR::ContainerType::Less: return "std::less";
		case IR::ContainerType::List: return "std::list";
		case IR::ContainerType::Map: return "std::map";
		case IR::ContainerType::MultiMap: return "std::multimap";
		case IR::ContainerType::MultiSet: return "std::multiset";
		case IR::ContainerType::Optional: return "std::optional";
		case IR::ContainerType::Pair: return "std::pair";
		case IR::ContainerType::PriorityQueue: return "std::priority_queue";
		case IR::ContainerType::Queue: return "std::queue";
		case IR::ContainerType::Set: return "std::set";
		case IR::ContainerType::SharedPtr: return "std::shared_ptr";
		case IR::ContainerType::Stack: return "std::stack";
		case IR::ContainerType::Tuple: return "std::tuple";
		case IR::ContainerType::UniquePtr: return "std::unique_ptr";
		case IR::ContainerType::UnorderedMap: return "std::unordered_map";
		case IR::ContainerType::UnorderedMultiMap:
			return "std::unordered_multimap";
		case IR::ContainerType::UnorderedMultiSet:
			return "std::unordered_multiset";
		case IR::ContainerType::UnorderedSet: return "std::unordered_set";
		case IR::ContainerType::Valarray: return "std::valarray";
		case IR::ContainerType::Variant: return "std::variant";
		case IR::ContainerType::Vector: return "std::vector";
	}
	return "";
}

}    // namespace Pybind::Helpers
