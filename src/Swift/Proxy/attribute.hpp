#pragma once

#include <string>
#include <vector>

namespace Swift::Proxy {
/**
* An attribute is a global value
*/
class Attribute {
public:
	Attribute(std::string const& name, std::string const& typeName);

	void addGetter(std::string const& body);

	void addSetter(std::string const& body);

	void setAsStatic();

	/**
	* Creates a string corresponding to the pybind11 conversion of this attribute.
	* E.g.
	*   attr("i") = &MyNamespace::i;
	*/
	std::string getSwift() const;

private:
	// The user defined name of the enum
	std::string m_name;
	std::string m_typeName;
	std::string m_getter;
	std::string m_setter;

	bool m_isStatic;
};
}    // namespace Swift::Proxy
