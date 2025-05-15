#pragma once

#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace IR {

// See https://en.cppreference.com/w/cpp/language/types for a list of builtin types
enum class BaseType {
	Bool,
	Char,
	Char16_t,
	Char32_t,
	Complex,
	Double,
	FilesystemPath,
	Float,
	Int,
	LongDouble,
	LongInt,
	LongLongInt,
	ShortInt,
	SignedChar,
	String,
	StringView,
	UnsignedChar,
	UnsignedInt,
	UnsignedLongInt,
	UnsignedLongLongInt,
	UnsignedShortInt,
	Void,
	Wchar_t,
};

enum class ContainerType {
	Allocator,
	Array,
	Deque,
	EqualTo,
	ForwardList,
	Greater,
	Hash,
	Less,
	List,
	Map,
	MultiMap,
	MultiSet,
	Optional,
	Pair,
	PriorityQueue,
	Queue,
	Set,
	SharedPtr,
	Stack,
	Tuple,
	UniquePtr,
	UnorderedMap,
	UnorderedMultiMap,
	UnorderedMultiSet,
	UnorderedSet,
	Valarray,
	Variant,
	Vector,
};

// https://en.cppreference.com/w/cpp/language/operators
enum class Operator {
	// +
	Addition,
	// -
	Subtraction,
	// *
	Multiplication,
	// /
	Division,
	// %
	Modulus,
	// =
	Assignment,
	// +=
	AddEqual,
	// -=
	SubEqual,
	// *=
	MulEqual,
	// /=
	DivEqual,
	// %=
	ModEqual,
	// ==
	Equal,
	// !=
	NotEqual,
	// >
	GreaterThan,
	// >=
	GreaterThanOrEqualTo,
	// <
	LessThan,
	// <=
	LessThanOrEqualTo,
	// <<
	LeftShift,
	// >>
	RightShift,
	// ++
	Increment,
	// --
	Decrement,
	// []
	Subscript,
	// ()
	Call
};

enum class Polymorphic {
	// Not Applicable
	NA,
	Virtual,
	PureVirtual,
};

struct Enum {
	std::string m_name;
	// Fully qualified name
	std::string m_representation;
	// Documentation string
	std::string m_documentation;
	// Unscoped values
	std::vector<std::string> m_values;

	size_t m_id;
	// True if it was declared as 'enum class'
	bool m_isScoped;
};

struct Type {
	struct Container {
		ContainerType m_container;
		std::vector<Type> m_containedTypes;
	};

	struct Value {
		BaseType m_base;
	};

	struct EnumValue {
		// Fully qualified name of the enum
		std::string m_representation;
	};

	struct UserDefined {
		// Fully qualified name of the struct
		std::string m_representation;
	};

	struct Function {
		// Fully qualified name of the function
		std::string m_representation;
	};

	// Represents a type parameter that is an integral
	// E.g. 3 in std::array<int, 3>
	struct Integral {};

	// Either it is a value with some type,
	// or it is a container ultimately containing a value type
	std::variant<Value, Container, EnumValue, UserDefined, Function, Integral>
	    m_type;

	// This is what the user wrote, removing any using's
	std::string m_representation;

	// Qualifiers
	bool m_isConst;

	// The numbers of pointers on this variable.
	// Ex:
	//      int* a;
	//        => m_numPointers = 1
	//      int*** a;
	//        => m_numPointers = 3
	int m_numPointers = 0;

	// Ex true for:
	//     std::string& someoneElsesString;
	bool m_isReference;
};

struct Argument {
	// Name that should be used to access the variable
	std::string m_name;

	Type m_type;
};

struct Variable {
	// Name that should be used to access the variable
	std::string m_name;

	Type m_type;

	// Documentation string
	std::string m_documentation;

	size_t m_id;

	bool m_isStatic;
};

struct Function {
	// Name that should be used to access the function
	std::string m_name;

	// This is the full name of the function
	// E.g. MyNamespace::myFunction
	std::string m_representation;

	// Documentation string
	std::string m_documentation;

	Type m_returnType;

	size_t m_id;

	std::vector<Argument> m_arguments;

	// Is empty if function is not a template
	std::vector<Type> m_templateArguments;

	Polymorphic m_polymorphic;

	bool m_isStatic;
};

struct Struct;

struct StructData {
	std::vector<Function> m_constructors;
	std::vector<Function> m_destructors;
	std::vector<Function> m_functions;
	std::vector<Variable> m_memberVariables;
	std::vector<Enum> m_enums;
	std::vector<Struct> m_structs;
	// Fully qualified name to inherited class
	std::vector<std::string> m_inherited;
	std::vector<std::pair<Operator, Function>> m_operators;
};

struct Struct {
	// NOTE: Structs are interpreted the same as classes
	// MyNamespace::MyClass -> m_name = MyClass
	std::string m_name;

	// This is the full name of the struct/class
	// E.g. MyNamespace::MyClass
	std::string m_representation;

	// Documentation string
	std::string m_documentation;

	StructData m_public;
	StructData m_private;
	StructData m_protected;

	// Is empty if struct is not a template
	std::vector<Type> m_templateArguments;

	size_t m_id;

	bool m_hasImplicitDefaultConstructor;
};

struct Namespace {
	// NOTE: The name might be "" if it is the global namespace
	std::string m_name;

	// This is the full name of the namespace
	// E.g. SomeNamespace::OtherNamespace::ThisNamespace
	std::string m_representation;

	// Documentation string
	std::string m_documentation;

	std::vector<Namespace> m_namespaces;
	std::vector<Struct> m_structs;
	std::vector<Function> m_functions;
	std::vector<Enum> m_enums;
	std::vector<Variable> m_variables;
};
}    // namespace IR
