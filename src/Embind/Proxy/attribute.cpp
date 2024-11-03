#include "Embind/Proxy/attribute.hpp"

#include <fmt/format.h>

#include <string>
#include <vector>

namespace Embind::Proxy {

Attribute::Attribute(std::string const& name,
                     std::string const& fullyQualifiedName)
  : m_name(name), m_fullyQualifiedName(fullyQualifiedName) {}

std::string Attribute::getEmbind(std::string const& namePrefix) const {
  return fmt::format("\tem::constant(\"{name}\", {fullyQualifiedName});\n",
                     fmt::arg("name", createName(namePrefix)),
                     fmt::arg("fullyQualifiedName", m_fullyQualifiedName));
}

std::string Attribute::getPreJS(std::string const& namePrefix,
                                std::vector<std::string>& namesToDelete) const {
  // Will no longer need the old name
  namesToDelete.push_back(createName(namePrefix));

  // Renaming the function
  // Expects to be injected where necessary
  return fmt::format("\t\t\t{baseName}: Module['{globalName}'],\n",
                     fmt::arg("baseName", m_name),
                     fmt::arg("globalName", namesToDelete.back()));
}

std::string Attribute::createName(std::string const& namePrefix) const {
  return namePrefix + m_name;
}

}    // namespace Embind::Proxy
