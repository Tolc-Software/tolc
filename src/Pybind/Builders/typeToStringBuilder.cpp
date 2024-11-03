#include <IR/ir.hpp>
#include <fmt/format.h>
#include <stack>
#include <string>
#include <variant>

namespace Pybind::Builders {

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
	return type.m_representation;
}

std::string toString(IR::ContainerType type) {
	using IR::ContainerType;

	switch (type) {
		case ContainerType::Array: return "array";
		case ContainerType::Deque: return "deque";
		case ContainerType::ForwardList: return "forwardlist";
		case ContainerType::List: return "list";
		case ContainerType::Map: return "map";
		case ContainerType::MultiMap: return "multimap";
		case ContainerType::MultiSet: return "multiset";
		case ContainerType::Optional: return "optional";
		case ContainerType::Pair: return "pair";
		case ContainerType::PriorityQueue: return "priorityqueue";
		case ContainerType::Queue: return "queue";
		case ContainerType::Set: return "set";
		case ContainerType::SharedPtr: return "sharedptr";
		case ContainerType::Stack: return "stack";
		case ContainerType::Tuple: return "tuple";
		case ContainerType::UniquePtr: return "uniqueptr";
		case ContainerType::UnorderedMap: return "unorderedmap";
		case ContainerType::UnorderedMultiMap: return "unorderedmultimap";
		case ContainerType::UnorderedMultiSet: return "unorderedmultiset";
		case ContainerType::UnorderedSet: return "unorderedset";
		case ContainerType::Valarray: return "valarray";
		case ContainerType::Variant: return "variant";
		case ContainerType::Vector: return "vector";

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
		case BaseType::Bool: return "bool";
		case BaseType::Char16_t: return "char16t";
		case BaseType::Char32_t: return "char32t";
		case BaseType::Char: return "char";
		case BaseType::Complex: return "complex";
		case BaseType::Double: return "double";
		case BaseType::FilesystemPath: return "string";
		case BaseType::Float: return "float";
		case BaseType::Int: return "int";
		case BaseType::LongDouble: return "longdouble";
		case BaseType::LongInt: return "longint";
		case BaseType::LongLongInt: return "longlongint";
		case BaseType::ShortInt: return "shortint";
		case BaseType::SignedChar: return "signedchar";
		case BaseType::String: return "string";
		case BaseType::StringView: return "stringview";
		case BaseType::UnsignedChar: return "unsignedchar";
		case BaseType::UnsignedInt: return "unsignedint";
		case BaseType::UnsignedLongInt: return "unsignedlongint";
		case BaseType::UnsignedLongLongInt: return "unsignedlonglongint";
		case BaseType::UnsignedShortInt: return "unsignedshortint";
		case BaseType::Void: return "void";
		case BaseType::Wchar_t: return "wchart";
	}
	return "";
}

std::string toString(IR::Type::Value const& type) {
	return toString(type.m_base);
}

void reverseAndAdd(std::vector<IR::Type> const& toReverse,
                   std::stack<IR::Type>& toAdd) {
	for (auto const& t : [&toReverse]() {
		     return std::vector<IR::Type>(toReverse.rbegin(), toReverse.rend());
	     }()) {
		toAdd.push(t);
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
	//   map
	//   |-- int
	//   `-- vector
	//       `-- char
	//   (Make string) ->
	//   map_int_vector_char

	std::stack<IR::Type> typesToStringify;
	// Reverse add so int comes before vector in the example above
	reverseAndAdd(type.m_containedTypes, typesToStringify);

	while (!typesToStringify.empty()) {
		auto current = typesToStringify.top();
		typesToStringify.pop();

		// Stringify this one

		std::visit(
		    overload {
		        [&typeString](IR::Type::Value type) {
			        typeString.push_back(toString(type));
		        },
		        [&typeString, &typesToStringify](IR::Type::Container type) {
			        // Some container type should not be entered (such as Allocator etc.)
			        // They are usually hidden from the user
			        if (auto container = toString(type.m_container);
			            !container.empty()) {
				        typeString.push_back(container);
				        reverseAndAdd(type.m_containedTypes, typesToStringify);
			        }
		        },
		        [&typeString](IR::Type::EnumValue type) {
			        typeString.push_back(toString(type));
		        },
		        [&typeString](IR::Type::UserDefined type) {
			        typeString.push_back(toString(type));
		        },
		        [&typeString](IR::Type::Function) {
			        // NOTE: Function to string is too complicated -> Give up
			        typeString.push_back("f");
		        },
		        [&typeString, &current](IR::Type::Integral) {
			        typeString.push_back(current.m_representation);
		        }},
		    current.m_type);
	}

	return fmt::format("{}", fmt::join(typeString, "_"));
}

std::string buildTypeString(IR::Type const& t) {
	std::vector<std::string> typeString;

	std::visit(
	    overload {[&typeString](IR::Type::Value type) {
		              if (auto s = toString(type); !s.empty()) {
			              typeString.push_back(toString(type));
		              }
	              },
	              [&typeString](IR::Type::Container type) {
		              if (auto s = toString(type); !s.empty()) {
			              typeString.push_back(toString(type));
		              }
	              },
	              [&typeString](IR::Type::EnumValue type) {
		              typeString.push_back(toString(type));
	              },
	              [&typeString](IR::Type::UserDefined type) {
		              typeString.push_back(toString(type));
	              },
	              [&typeString](IR::Type::Function) {
		              // NOTE: Function to string is too complicated -> Give up
		              typeString.push_back("f");
	              },
	              [&typeString, &t](IR::Type::Integral) {
		              typeString.push_back(t.m_representation);
	              }},
	    // , std::variant<Value, Container, EnumValue, UserDefined, Function, Integral> m_type;
	    t.m_type);

	return fmt::format("{}", fmt::join(typeString, "_"));
}
}    // namespace Pybind::Builders
