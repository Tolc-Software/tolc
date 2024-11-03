#pragma once

#include <string>
#include <vector>

namespace Embind::Proxy {
/**
* An attribute is a global value
*/
class Attribute {
  public:
  Attribute(std::string const& name, std::string const& fullyQualifiedName);

  /**
	* Creates a string corresponding to the embind conversion of this attribute.
	* E.g.
	*   em::constant("myGlobal", myGlobal)
	*/
  std::string getEmbind(std::string const& namePrefix = "") const;

  std::string getPreJS(std::string const& namePrefix,
                       std::vector<std::string>& namesToDelete) const;

  private:
  // The user defined name of the enum
  std::string m_name;
  std::string m_fullyQualifiedName;

  std::string createName(std::string const& namePrefix) const;
};
}    // namespace Embind::Proxy
