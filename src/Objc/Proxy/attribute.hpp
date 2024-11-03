#pragma once

#include "Objc/Proxy/structure.hpp"
#include "Objc/Proxy/type.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {
/**
* An attribute is either a global value or a member variable
*/
struct Attribute : public Structure {
	explicit Attribute(std::string const& cppClass,
	                   std::string const& objcClass,
	                   std::string const& name);

	std::string getObjcHeader() const override;
	std::string getObjcSource() const override;

	std::string const& getName() const;

	void setType(Objc::Proxy::Type const& type);
	Objc::Proxy::Type const& getType() const;

	void setDocumentation(std::string const& documentation);

	void setAsConst();
	bool isConst() const;

	void setAsStatic();
	bool isStatic() const;

	void setAsObject();

	void setAsStandalone();
	bool isStandalone() const;

private:
	// private?
	std::string getPropertyOptions() const;
	// User defined name of the member variable
	std::string m_name;
	// The name of the surrounding class
	std::string m_cppClass;
	std::string m_objcClass;
	Objc::Proxy::Type m_type;
	std::string m_documentation;
	bool m_isConst;
	bool m_isStatic;
	// Determines which memory property it should have
	bool m_isObject;
	// If it needs wrappers or not
	bool m_isStandalone;
};
}    // namespace Objc::Proxy
