#pragma once

#include <optional>
#include <string>
#include <vector>

namespace Pybind::Proxy {
class Function {
public:
	Function(std::string const& name, std::string const& fullyQualifiedName);

	/**
	* Creates a string corresponding to the pybind11 conversion of this function.
	* E.g.
	*   def("myFunction", &SomeNamespace::myFunction)
	*/
	std::string getPybind() const;

	/**
	* Adds an argument name.
	* E.g.
	*   f(int i) would require addArgument("int", "i")
	*/
	void addArgument(std::string const& typeName, std::string const& name = "");

	/**
	* Documentation for the corresponding C++ function
	* E.g. Doxygen comment
	*/
	void setDocumentation(std::string const& documentation);

	void setAsConstructor();

	// Is there another function with the same name but with different arguments?
	void setAsOverloaded();

	void setAsStatic();

	// E.g. std::string f();
	// should call setReturnType("std::string")
	void setReturnType(std::string const& typeName);

	// As it will be called in python
	void setPythonName(std::string const& name);
	std::string getPythonName() const;

	std::string getName() const;

	std::string getReturnType() const;

	std::string getArgumentTypes(bool withNames = false) const;
	std::string getArgumentNames() const;

	// These are all 1-to-1 with pybind11
	enum class return_value_policy {
		take_ownership,
		copy,
		move,
		reference,
		reference_internal,
		automatic,
		automatic_reference
	};

	/**
	* Sets the return value policy for the function.
	* E.g.
	*   An input of "return_value_policy::take_ownership"
	*   would result in python calling delete on the object
	*   when it's garbage collected
	* NOTE: If not put in, the return_value_policy::automatic is used
	*/
	void setReturnValuePolicy(return_value_policy policy);

private:
	std::string getSignature() const;

	struct Argument {
		// E.g.
		//   f(int i)
		// Would result in
		//   typeName = "int"
		//   name = "i"
		std::string typeName;
		std::string name;
	};

	// User defined name of the function
	std::string m_name;
	// Override the C++ name
	// E.g. for operator names
	std::optional<std::string> m_pythonName;
	std::string m_fullyQualifiedName;
	std::string m_documentation;
	// Defaults to void
	std::string m_returnType;
	std::optional<return_value_policy> m_returnValuePolicy;
	std::vector<Argument> m_arguments;
	bool m_isConstructor;
	bool m_isOverloaded;
	bool m_isStatic;
};
}    // namespace Pybind::Proxy
