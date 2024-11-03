#pragma once

#include "Objc/Proxy/attribute.hpp"
#include "Objc/Proxy/enum.hpp"
#include "Objc/Proxy/function.hpp"
#include "Objc/Proxy/structure.hpp"
#include "Objc/Proxy/type.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {

class Class : public Structure {
public:
	explicit Class(std::string const& name,
	               std::string const& fullyQualifiedName);

	std::string getObjcSource() const override;
	std::string getObjcHeader() const override;

	void addEnum(Enum const& e);
	std::vector<Enum> const& getEnums() const;

	void addFunction(Function const& function);
	std::vector<Function> const& getFunctions() const;

	void addConstructor(Function const& constructor);
	std::vector<Function> const& getConstructors() const;

	void addMemberVariable(Attribute const& variable);
	std::vector<Attribute> const& getAttributes() const;

	std::string const& getName() const;

	void setDocumentation(std::string const& documentation);

	void setInherited(std::vector<std::string> const& inherited);

	// Will be managed by a std::shared_ptr on the Objective-C side
	// instead of the default std::unique_ptr
	void setAsManagedByShared();

	// Meaning it has no underlying C++ class
	void setAsPurelyStatic();

	std::string const& getCppClassName() const;

	// Get the extension of this class
	std::string declareCategory() const;

private:
	std::string joinMemberVariables(bool isSource) const;

	// User defined name of the class
	std::string m_name;
	std::string m_fullyQualifiedName;
	std::string m_documentation;

	std::vector<std::string> m_inherited;
	std::vector<Function> m_constructors;
	std::vector<Function> m_functions;
	std::vector<Attribute> m_memberVariables;
	std::vector<Enum> m_enums;

	bool m_isManagedByShared;
	bool m_isPurelyStatic;
};
}    // namespace Objc::Proxy
