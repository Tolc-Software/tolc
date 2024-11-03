#pragma once

#include <string>
#include <vector>

namespace Embind::Proxy {
class Enum {
  public:
  Enum(std::string const& name, std::string const& fullyQualifiedName);

  /**
	* Creates a string corresponding to the embind conversion of this enum.
	* E.g.
	*   enum_<Pet::Kind>("Kind").value("Dog", Pet::Kind::Dog)
	*/
  std::string getEmbind(std::string const& namePrefix = "") const;

  std::string getPreJS(std::string const& namePrefix,
                       std::vector<std::string>& namesToDelete) const;

  /**
	* Add an enum value. Should be just the value name.
	* E.g.
	*   enum class MyEnum {
	*      MyValue;
	*   };
	*   Should only add the value 'MyValue' and not 'MyEnum::MyValue'
	*/
  void addValue(std::string const& value);

  // Corresponds to the variable name on the Module Object
  std::string createName(std::string const& namePrefix) const;

  // Get the simple name of the enum
  std::string getName() const;

  private:
  // The user defined name of the enum
  std::string m_name;
  std::string m_fullyQualifiedName;
  std::vector<std::string> m_values;
};
}    // namespace Embind::Proxy
