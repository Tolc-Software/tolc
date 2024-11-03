#include "Objc/Builders/typeBuilder.hpp"
#include "Objc/Conversions/base.hpp"
#include "Objc/Conversions/container.hpp"
#include "Objc/Conversions/conversion.hpp"
#include "Objc/Conversions/getConversionName.hpp"
#include "Objc/Conversions/userDefined.hpp"
#include "Objc/cache.hpp"
#include "Objc/getName.hpp"
#include <IR/ir.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <variant>

namespace Objc::Builders {

Proxy::Nullability getNullable(IR::BaseType base, bool hasPointers) {
	using IR::BaseType;
	switch (base) {
		case BaseType::String:
		case BaseType::StringView:
		case BaseType::FilesystemPath: {
			if (hasPointers) {
				return Proxy::Nullability::Nullable;
			}
			return Proxy::Nullability::Nonnull;
		}
		case BaseType::Bool:
		case BaseType::Char16_t:
		case BaseType::Char32_t:
		case BaseType::Char:
		case BaseType::Complex:
		case BaseType::Double:
		case BaseType::Float:
		case BaseType::Int:
		case BaseType::LongDouble:
		case BaseType::LongInt:
		case BaseType::LongLongInt:
		case BaseType::ShortInt:
		case BaseType::SignedChar:
		case BaseType::UnsignedChar:
		case BaseType::UnsignedInt:
		case BaseType::UnsignedLongInt:
		case BaseType::UnsignedLongLongInt:
		case BaseType::UnsignedShortInt:
		case BaseType::Void:
		case BaseType::Wchar_t: {
			return Proxy::Nullability::NotApplicable;
		}
	}
}
Proxy::Nullability getNullable(IR::ContainerType container, bool hasPointers) {
	using IR::ContainerType;
	switch (container) {
		case ContainerType::Array:
		case ContainerType::Deque:
		case ContainerType::List:
		case ContainerType::Pair:
		case ContainerType::Tuple:
		case ContainerType::Valarray:
		case ContainerType::Vector:
		case ContainerType::Map:
		case ContainerType::UnorderedMap:
		case ContainerType::Set:
		case ContainerType::UnorderedSet: {
			if (hasPointers) {
				return Proxy::Nullability::Nullable;
			}
			return Proxy::Nullability::Nonnull;
		}
		case ContainerType::Optional:
		case ContainerType::SharedPtr:
		case ContainerType::UniquePtr: {
			return Proxy::Nullability::Nullable;
		}
		case ContainerType::UnorderedMultiSet:
		case ContainerType::Queue:
		case ContainerType::MultiMap:
		case ContainerType::MultiSet:
		case ContainerType::PriorityQueue:
		case ContainerType::Stack:
		case ContainerType::UnorderedMultiMap:
		case ContainerType::Variant:
		case ContainerType::Allocator:
		case ContainerType::EqualTo:
		case ContainerType::ForwardList:
		case ContainerType::Greater:
		case ContainerType::Hash:
		case ContainerType::Less: {
			return Proxy::Nullability::NotApplicable;
		}
	}
}

Objc::Proxy::Type buildType(IR::Type const& type, Objc::Cache& cache) {
	Objc::Proxy::Type t;
	t.m_cppType = &type;
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_type)) {
		auto name = Objc::getBaseName(baseType->m_base);
		t.m_nullability =
		    getNullable(baseType->m_base, type.m_numPointers != 0);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions =
		    Objc::Conversions::getBaseTypeConversions(baseType->m_base, cache);
	} else if (auto enumType = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
		t.m_nullability = type.m_numPointers != 0 ?
                              Proxy::Nullability::Nullable :
                              Proxy::Nullability::Nonnull;
		auto name =
		    Objc::getEnumName(enumType->m_representation, cache.m_moduleName);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions = Objc::Conversions::getEnumConversionNames(
		    cache.m_moduleName,
		    enumType->m_representation,
		    cache.m_extraFunctionsNamespace);
	} else if (auto container =
	               std::get_if<IR::Type::Container>(&type.m_type)) {
		auto name =
		    Objc::getContainerName(*container, cache.m_moduleName);
		t.m_nullability =
		    getNullable(container->m_container, type.m_numPointers != 0);
		t.m_name = [name]() {
			return name;
		};
		t.m_conversions = Objc::Conversions::getContainerTypeConversions(
		    type, *container, cache);
	} else if (auto userDefined =
	               std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		t.m_nullability = type.m_numPointers != 0 ?
                              Proxy::Nullability::Nullable :
                              Proxy::Nullability::Nonnull;
		t.m_name = [cppClass = userDefined->m_representation, &cache]() {
			if (auto objcClass = cache.m_cppToObjcClassNames.find(cppClass);
			    objcClass != cache.m_cppToObjcClassNames.end()) {
				return objcClass->second + '*';
			}
			spdlog::error(
			    "Couldn't find the Objective-C class name of C++ class {}",
			    cppClass);
			return std::string();
		};
		t.m_conversions = Objc::Conversions::getUserDefinedConversionNames(
		    userDefined->m_representation, cache.m_extraFunctionsNamespace);
	}

	t.m_dereference = "";
	for (int i = 0; i < type.m_numPointers; ++i) {
		t.m_dereference.push_back('*');
	}
	return t;
}

}    // namespace Objc::Builders
