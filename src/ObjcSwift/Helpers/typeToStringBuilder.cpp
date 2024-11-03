#include <IR/ir.hpp>
#include <fmt/format.h>
#include <stack>
#include <string>
#include <variant>

namespace ObjcSwift::Helpers {

// Magic template stuff to make std::visit work nicely with std::variant
// https://en.cppreference.com/w/cpp/utility/variant/visit
template <class... Ts>
struct overload : Ts... {
	using Ts::operator()...;
};
template <class... Ts>
overload(Ts...) -> overload<Ts...>;    // line not needed in C++20...

std::string toString(IR::Type::EnumValue const& type) {
	return type.m_representation;
}

std::string toString(IR::Type::UserDefined const& type) {
	// Safeguard for mistakes in the parser
	return type.m_representation.starts_with("std::") ? "" :
                                                        type.m_representation;
}

std::string toString(IR::ContainerType type) {
	using IR::ContainerType;

	switch (type) {
		case ContainerType::Array: return "Array";
		case ContainerType::Deque: return "Deque";
		case ContainerType::ForwardList: return "ForwardList";
		case ContainerType::List: return "List";
		case ContainerType::Map: return "Map";
		case ContainerType::MultiMap: return "MultiMap";
		case ContainerType::MultiSet: return "MultiSet";
		case ContainerType::Optional: return "Optional";
		case ContainerType::Pair: return "Pair";
		case ContainerType::PriorityQueue: return "PriorityQueue";
		case ContainerType::Queue: return "Queue";
		case ContainerType::Set: return "Set";
		case ContainerType::SharedPtr: return "SharedPtr";
		case ContainerType::Stack: return "Stack";
		case ContainerType::Tuple: return "Tuple";
		case ContainerType::UniquePtr: return "UniquePtr";
		case ContainerType::UnorderedMap: return "UnorderedMap";
		case ContainerType::UnorderedMultiMap: return "UnorderedMultiMap";
		case ContainerType::UnorderedMultiSet: return "UnorderedMultiSet";
		case ContainerType::UnorderedSet: return "UnorderedSet";
		case ContainerType::Valarray: return "Valarray";
		case ContainerType::Variant: return "Variant";
		case ContainerType::Vector: return "Vector";

		// These are usually "hidden" within defaulted template parameters in e.g. vector, so no generated string
		case ContainerType::Allocator:
		case ContainerType::EqualTo:
		case ContainerType::Greater:
		case ContainerType::Hash:
		case ContainerType::Less: break;
	}

	return "";
}

std::string toString(IR::BaseType type) {
	using IR::BaseType;
	switch (type) {
		case BaseType::Bool: return "Bool";
		case BaseType::Char16_t: return "Char16t";
		case BaseType::Char32_t: return "Char32t";
		case BaseType::Char: return "Char";
		case BaseType::Complex: return "Complex";
		case BaseType::Double: return "Double";
		case BaseType::FilesystemPath: return "String";
		case BaseType::Float: return "Float";
		case BaseType::Int: return "Int";
		case BaseType::LongDouble: return "LongDouble";
		case BaseType::LongInt: return "Longint";
		case BaseType::LongLongInt: return "LongLongInt";
		case BaseType::ShortInt: return "ShortInt";
		case BaseType::SignedChar: return "SignedChar";
		case BaseType::String: return "String";
		case BaseType::StringView: return "StringView";
		case BaseType::UnsignedChar: return "UnsignedChar";
		case BaseType::UnsignedInt: return "UnsignedInt";
		case BaseType::UnsignedLongInt: return "UnsignedLongInt";
		case BaseType::UnsignedLongLongInt: return "UnsignedLongLongInt";
		case BaseType::UnsignedShortInt: return "UnsignedShortInt";
		case BaseType::Void: return "Void";
		case BaseType::Wchar_t: return "Wchart";
	}
	return "";
}

std::string toString(IR::Type::Value const& type) {
	return toString(type.m_base);
}

void reverseAndAdd(std::vector<IR::Type> const& toReverse,
                   std::stack<IR::Type const*>& toAdd) {
	for (auto it = toReverse.rbegin(); it != toReverse.rend(); ++it) {
		toAdd.push(&(*it));
	}
}

std::string toString(IR::Type::Container const& type) {
	// Start with the base container
	auto start = toString(type.m_container);
	std::vector<std::string> typeString = start.empty() ?
                                              std::vector<std::string>() :
                                              std::vector<std::string> {start};

	// Go depth first down through the types
	// Ex:
	//   map<int, vector<char>>
	//   (Make tree) ->
	//   Map
	//   |-- Int
	//   `-- Vector
	//       `-- Char
	//   (Make string) ->
	//   MapIntVectorChar

	std::stack<IR::Type const*> typesToStringify;
	// Reverse add so int comes before vector in the example above
	reverseAndAdd(type.m_containedTypes, typesToStringify);

	while (!typesToStringify.empty()) {
		auto current = typesToStringify.top();
		typesToStringify.pop();

		// Stringify this one

		std::visit(
		    overload {
		        [&typeString](IR::Type::Value const& type) {
			        typeString.push_back(toString(type));
		        },
		        [&typeString,
		         &typesToStringify](IR::Type::Container const& type) {
			        // Some container type should not be entered (such as Allocator etc.)
			        // They are usually hidden from the user
			        if (auto container = toString(type.m_container);
			            !container.empty()) {
				        typeString.push_back(container);
				        reverseAndAdd(type.m_containedTypes, typesToStringify);
			        }
		        },
		        [&typeString](IR::Type::EnumValue const& type) {
			        typeString.push_back(toString(type));
		        },
		        [&typeString](IR::Type::UserDefined const& type) {
			        typeString.push_back(toString(type));
		        },
		        [&typeString](IR::Type::Function) {
			        // NOTE: Function to string is too complicated -> Give up
			        typeString.push_back("F");
		        },
		        [&typeString, &current](IR::Type::Integral) {
			        typeString.push_back(current->m_representation);
		        }},
		    current->m_type);
	}

	return fmt::format("{}", fmt::join(typeString, ""));
}

std::string buildTypeString(IR::Type const& t) {
	std::string typeString;

	std::visit(
	    overload {[&typeString](IR::Type::Value const& type) {
		              if (auto s = toString(type); !s.empty()) {
			              typeString += toString(type);
		              }
	              },
	              [&typeString](IR::Type::Container const& type) {
		              if (auto s = toString(type); !s.empty()) {
			              typeString += toString(type);
		              }
	              },
	              [&typeString](IR::Type::EnumValue const& type) {
		              typeString += toString(type);
	              },
	              [&typeString](IR::Type::UserDefined const& type) {
		              typeString += toString(type);
	              },
	              [&typeString](IR::Type::Function) {
		              // NOTE: Function to string is too complicated -> Give up
		              typeString += "F";
	              },
	              [&typeString, &t](IR::Type::Integral) {
		              typeString += t.m_representation;
	              }},
	    // , std::variant<Value, Container, EnumValue, UserDefined, Function, Integral> m_type;
	    t.m_type);

	return fmt::format("{}", typeString);
}
}    // namespace ObjcSwift::Helpers
