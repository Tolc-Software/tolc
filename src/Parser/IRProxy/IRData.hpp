#pragma once

#include <IR/ir.hpp>
#include <deque>
#include <map>
#include <set>
#include <string>
#include <vector>

namespace IRProxy {

enum class AccessModifier { Public, Private, Protected };

/**
  * Used to distinguish a part name in a fully qualified name
  * Ex:
  *     NamespaceName::ClassName -> {(NamespaceName, Structure::Namespace)
  *                                  (ClassName, Structure::Struct)}
  */
enum class Structure {
	Namespace,
	Struct,
	Function,
	Enum,
};

struct StructData {
	std::vector<IR::Variable> m_variables;
	std::vector<std::string> m_inherited;
};

/**
  * Representation of a struct/class that can be used to create the IR::Struct
  */
struct Struct {
	std::string m_fullyQualifiedName;
	std::string m_documentation;

	// Fully qualified name with structure annotations
	// Path to where this struct belongs
	// Ex:
	//     Ns::cl => {(Ns, Structure::Namespace), (cl, Structure::Struct)}
	std::deque<std::pair<std::string, Structure>> m_path;

	// The variables within the struct/class
	StructData m_public;
	StructData m_private;
	StructData m_protected;

	// Empty if not a template
	std::vector<IR::Type> m_templateArguments;

	// Unique for this object
	size_t m_id;

	// public, private, protected
	// For nested structs
	std::optional<IRProxy::AccessModifier> m_modifier;

	bool m_hasImplicitDefaultConstructor;
};

/**
  * Representation of a function that can be used to create the IR::Function
  */
struct Function {
	std::string m_fullyQualifiedName;

	std::string m_documentation;

	// Fully qualified name with structure annotations
	// Path to where this function belongs
	// Ex:
	//     Ns::cl::fun => {(Ns, Structure::Namespace), (cl, Structure::Struct), (cl, Structure::Function)}
	std::deque<std::pair<std::string, Structure>> m_path;

	std::vector<IR::Argument> m_arguments;

	// Empty if not a template
	std::vector<IR::Type> m_templateArguments;

	// public, private, protected
	std::optional<IRProxy::AccessModifier> m_modifier;

	// Virtual, purevirtual or not applicable
	IR::Polymorphic m_polymorphic;

	// Operator overloading
	std::optional<IR::Operator> m_operator;

	IR::Type m_returnType;

	// Unique for this object
	size_t m_id;

	bool m_isStatic;

	bool m_isConstructor;
	bool m_isDestructor;
};

/**
  * Representation of a enum that can be used to create the IR::Enum
  */
struct Enum {
	std::string m_fullyQualifiedName;
	std::string m_documentation;

	// Fully qualified name with structure annotations
	// Path to where this struct belongs
	// Ex:
	//     Ns::cl => {(Ns, Structure::Namespace), (cl, Structure::Struct)}
	std::deque<std::pair<std::string, Structure>> m_path;

	// public, private, protected
	std::optional<IRProxy::AccessModifier> m_modifier;

	// Unique for this object
	size_t m_id;

	// Unscoped values of the enum
	std::vector<std::string> m_values;

	bool m_isScoped;
};

/**
  * Representation of a variable within a struct/class
  */
struct MemberVariable {
	IR::Variable m_variable;

	// public, private, protected
	IRProxy::AccessModifier m_modifier;
};

/**
  * Representation of a namespace
  */
struct Namespace {
	std::string m_fullyQualifiedName;

	std::string m_documentation;
};

/**
  * Contains partially parsed code to IR on the form
  *     m_something = vector<[parent context, IR::something]>
  *
  * NOTE: Each 'm_something' should ONLY be touched by their corresponding visit function.
  *       Ex:
  *           m_namespaces should only be added to in the VisitNamespaceDecl function
  *
  * NOTE: The parent context might be a null pointer or otherwise empty
  *       if 'something' is in the global scope
  */
struct IRData {
	// {Fully qualified namespace name}
	// Ex: ParentNS::ChildNS::GrandChildNS
	std::vector<Namespace> m_namespaces;

	std::vector<Struct> m_structs;

	std::vector<Function> m_functions;

	std::vector<Enum> m_enums;

	// {Fully qualified name of the owning class: variable}
	std::map<std::string, std::vector<MemberVariable>> m_memberVariables;

	// {Fully qualified name of the owning namespace: variable}
	std::map<std::string, std::vector<IR::Variable>> m_globalVariables;

	// {id: [dependent ids]}
	std::vector<std::set<size_t>> m_dependencyMap;
	// Only contains Enums and User defined structs
	// {Representation: id}
	std::map<std::string, size_t> m_idMap;
};
}    // namespace IRProxy
