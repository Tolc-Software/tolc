#include "Objc/getName.hpp"
#include "ObjcSwift/Helpers/split.hpp"
#include "ObjcSwift/Helpers/typeToStringBuilder.hpp"
#include "ObjcSwift/Helpers/types.hpp"
#include <IR/ir.hpp>
#include <fmt/format.h>
#include <numeric>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <vector>

namespace Objc {
namespace {

std::string joinVariableName(std::string qualifiedName,
                             std::string const& rootModuleName) {
	// MyNS::Math, rootModule -> rootModuleMyNsMath
	// This is to avoid naming conflicts when defining namespaces with the
	// same name as the root module
	// This happens if you call your module tensorflow and have a namespace with tensorflow
	auto splitted = ObjcSwift::Helpers::split(qualifiedName, "::");

	splitted.push_front(rootModuleName);
	// If qualifiedName is the root name (global namespace has no name)
	// This will return rootModuleName
	return fmt::format("{}", fmt::join(splitted, ""));
}

std::string getParameterString(std::vector<IR::Type> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, auto const& current) {
		                       return std::move(soFar) +
		                              ObjcSwift::Helpers::buildTypeString(
		                                  current);
	                       });
}

std::string valueContainerName(IR::BaseType base) {
	using IR::BaseType;
	switch (base) {
		case BaseType::Bool:
		case BaseType::Int:
		case BaseType::Char:
		case BaseType::Double:
		case BaseType::Float:
		case BaseType::LongInt:
		case BaseType::LongLongInt:
		case BaseType::ShortInt:
		case BaseType::SignedChar:
		case BaseType::UnsignedChar:
		case BaseType::UnsignedInt:
		case BaseType::UnsignedLongInt:
		case BaseType::UnsignedLongLongInt:
		case BaseType::UnsignedShortInt: {
			return "NSNumber*";
		}
		case BaseType::String:
		case BaseType::FilesystemPath:
		case BaseType::StringView: {
			return "NSString*";
		}
		case BaseType::Void: {
			break;
		}
		case BaseType::Wchar_t: {
			break;
		}
		case BaseType::Char16_t: {
			break;
		}
		case BaseType::Char32_t: {
			break;
		}
		case BaseType::LongDouble: {
			break;
		}
		case BaseType::Complex: {
			break;
		}
	}
	return "Unknown base type name";
}

}    // namespace

std::string getParameterString(std::vector<IR::Argument> const& parameters) {
	return std::accumulate(parameters.begin(),
	                       parameters.end(),
	                       std::string() /* Start with empty string */,
	                       [](std::string soFar, auto const& current) {
		                       return std::move(soFar) +
		                              ObjcSwift::Helpers::buildTypeString(
		                                  current.m_type);
	                       });
}

std::string getConstructorExtraName(IR::Function const& f) {
	if (f.m_arguments.empty()) {
		return "";
	}
	return "With" + getParameterString(f.m_arguments);
}

std::string getContainedTypeName(IR::Type const& type,
                                 std::string const& moduleName) {
	if (auto baseType = std::get_if<IR::Type::Value>(&type.m_type)) {
		return valueContainerName(baseType->m_base);
	} else if (auto enumType = std::get_if<IR::Type::EnumValue>(&type.m_type)) {
		return Objc::getEnumName(enumType->m_representation, moduleName);
	} else if (auto container =
	               std::get_if<IR::Type::Container>(&type.m_type)) {
		return Objc::getContainerName(*container, moduleName);
	} else if (auto userDefined =
	               std::get_if<IR::Type::UserDefined>(&type.m_type)) {
		return Objc::getClassName(userDefined->m_representation, moduleName) +
		       '*';
	}
	return "";
}


std::string getClassName(std::string const& cppClassFqName,
                         std::string const& moduleName) {
	// TODO: Handle templateArgs
	auto [fqName, _templateArgs] =
	    ObjcSwift::Helpers::removeCppTemplate(cppClassFqName);
	return fmt::format(
	    "{moduleName}{className}{templateArgs}",
	    fmt::arg("moduleName", moduleName),
	    fmt::arg("className",
	             fmt::join(ObjcSwift::Helpers::split(fqName, "::"), "")),
	    fmt::arg("templateArgs", _templateArgs));
}

std::string getClassName(IR::Struct const& cppClass,
                         std::string const& moduleName) {
	auto withoutTemplate =
	    ObjcSwift::Helpers::removeCppTemplate(cppClass.m_representation).first;

	return fmt::format(
	    "{moduleName}{className}{templateArgs}",
	    fmt::arg("moduleName", moduleName),
	    fmt::arg(
	        "className",
	        fmt::join(ObjcSwift::Helpers::split(withoutTemplate, "::"), "")),
	    fmt::arg("templateArgs",
	             getParameterString(cppClass.m_templateArguments)));
}

std::string getFunctionName(IR::Function const& cppFunction,
                            bool isConstructor,
                            bool isOverloaded) {
	if (!isConstructor) {
		auto objcName =
		    ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name).first;
		if (isOverloaded) {
			objcName += Objc::getParameterString(cppFunction.m_arguments);
		}
		return fmt::format(
		    "{funcionName}{paramString}{templateArgs}",
		    fmt::arg("funcionName",
		             ObjcSwift::Helpers::removeCppTemplate(cppFunction.m_name)
		                 .first),
		    fmt::arg("paramString",
		             isOverloaded ?
                         Objc::getParameterString(cppFunction.m_arguments) :
                         ""),
		    fmt::arg("templateArgs",
		             getParameterString(cppFunction.m_templateArguments)));
	} else {
		return "init" + Objc::getConstructorExtraName(cppFunction);
	}
}

std::string getEnumName(std::string const& qualifiedEnumName,
                        std::string const& moduleName) {
	return Objc::joinVariableName(qualifiedEnumName, moduleName);
}

std::string getBaseName(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "bool";
		case BaseType::Char16_t: return "char16_t";
		case BaseType::Char32_t: return "char32_t";
		case BaseType::Char: return "char";
		case BaseType::Complex: return "complex";
		case BaseType::Double: return "double";
		case BaseType::String:
		case BaseType::StringView:
		case BaseType::FilesystemPath: return "NSString*";
		case BaseType::Float: return "float";
		case BaseType::Int: return "int";
		case BaseType::LongDouble: return "long double";
		case BaseType::LongInt: return "long int";
		case BaseType::LongLongInt: return "long long int";
		case BaseType::ShortInt: return "short int";
		case BaseType::SignedChar: return "signed char";
		case BaseType::UnsignedChar: return "unsigned char";
		case BaseType::UnsignedInt: return "unsigned int";
		case BaseType::UnsignedLongInt: return "unsigned long int";
		case BaseType::UnsignedLongLongInt: return "unsigned long long int";
		case BaseType::UnsignedShortInt: return "unsigned short int";
		case BaseType::Void: return "void";
		case BaseType::Wchar_t: return "wchar_t";
	}
	return "";
}

std::string getContainerName(IR::Type::Container const& container,
                             std::string const& moduleName) {
	using IR::ContainerType;
	switch (container.m_container) {
		case ContainerType::Array:
		case ContainerType::Deque:
		case ContainerType::List:
		case ContainerType::Pair:
		case ContainerType::Tuple:
		case ContainerType::Valarray:
		case ContainerType::Vector: return "NSArray*";

		case ContainerType::Map:
		case ContainerType::UnorderedMap: return "NSDictionary*";

		case ContainerType::Set: return "NSOrderedSet*";

		case ContainerType::UnorderedSet: return "NSSet*";

		case ContainerType::Optional:
		case ContainerType::SharedPtr:
		case ContainerType::UniquePtr:
			return Objc::getContainedTypeName(
			    container.m_containedTypes.front(), moduleName);

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
			spdlog::error("Unsupported containerType detected.");
			return "Unsupported container type";
		}
	}
}
std::string getNullableName(Objc::Proxy::Type const& type) {
	switch (type.m_nullability) {
		case Objc::Proxy::Nullability::Nullable:
			// Do when Objc::Proxy::Nullability::Nullable
			return "nullable " + type.m_name();
		case Objc::Proxy::Nullability::Nonnull:
			// Do when Objc::Proxy::Nullability::Nonnull
			return "nonnull " + type.m_name();
		case Objc::Proxy::Nullability::NotApplicable: break;
	}
	return type.m_name();
}
}    // namespace Objc
