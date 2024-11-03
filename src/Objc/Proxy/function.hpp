#pragma once

#include "Objc/Proxy/structure.hpp"
#include "Objc/Proxy/type.hpp"
#include <optional>
#include <string>
#include <vector>

namespace Objc::Proxy {
class Function : public Structure {
public:
	Function(std::string const& name,
	         std::string const& cppName,
	         std::string const& fullyQualifiedName,
	         std::string const& objcClass,
	         std::string const& cppClass);

	std::string getObjcSource() const override;
	std::string getObjcHeader() const override;

	/**
	* Documentation for the corresponding C++ function
	* E.g. Doxygen comment
	*/
	void setDocumentation(std::string const& documentation);

	// Is there another function with the same name but with different arguments?
	// Then add a unique index for this function
	void setAsOverloaded(size_t overloadIndex);

	void setAsStatic();
	bool isStatic() const;

	void setAsConstructor();
	bool isConstructor() const;

	void setReturnType(Objc::Proxy::Type const& type);
	Objc::Proxy::Type const& getReturnType() const;

	std::string getName() const;
	// Gets the fully qualified name of the C++ function
	std::string getCppName() const;

	struct Argument {
		std::string m_name;
		Objc::Proxy::Type m_type;
	};

	void addArgument(Argument const& arg);
	std::vector<Argument> const& getArguments() const;

	void setAsStandalone();
	bool isStandalone() const;

private:
	std::string getFunctionCall() const;
	std::string getFunctionDeclaration() const;
	std::string getFunctionBody() const;

	// User defined name of the function
	std::string m_name;
	std::string m_cppName;
	std::string m_fullyQualifiedName;
	// Containing class
	std::string m_objcClass;
	std::string m_cppClass;
	std::string m_documentation;
	// Defaults to void
	Objc::Proxy::Type m_returnType;
	std::vector<Argument> m_arguments;
	// Gets added to the function category if overloaded
	std::optional<size_t> m_overloadedIndex;
	bool m_isStatic;
	bool m_isConstructor;
	// If it needs wrappers or not
	bool m_isStandalone;
};
}    // namespace Objc::Proxy
