#pragma once

#include "Objc/Proxy/structure.hpp"
#include <string>
#include <vector>

namespace Objc::Proxy {
class Enum : public Structure {
public:
	Enum(std::string const& name);

	std::string getObjcSource() const override;

	std::string getObjcHeader() const override;

	/**
	* Add an enum value. Should be just the value name.
	* E.g.
	*   enum class MyEnum {
	*      MyValue;
	*   };
	*   Should only add the value 'MyValue' and not 'MyEnum::MyValue'
	*/
	void addValue(std::string const& value);

	std::string getName() const;
	std::vector<std::string> getValues() const;

	void setScoped(bool isScoped);

	void setDocumentation(std::string const& documentation);

private:
	// The user defined name of the enum
	std::string m_name;
	std::string m_documentation;
	std::vector<std::string> m_values;
	bool m_isScoped;
};
}    // namespace Objc::Proxy
