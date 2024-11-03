#include "Embind/Proxy/enum.hpp"

#include <fmt/format.h>

#include <string>
#include <vector>

namespace Embind::Proxy {

Enum::Enum(std::string const& name, std::string const& fullyQualifiedName)
  : m_name(name), m_fullyQualifiedName(fullyQualifiedName), m_values({}) {};

std::string Enum::getEmbind(std::string const& namePrefix) const {
  std::string out =
      fmt::format("\tem::enum_<{fullyQualifiedName}>(\"{name}\")\n",
                  fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
                  fmt::arg("name", createName(namePrefix)));

  for (auto const& value : m_values) {
    out +=
        fmt::format("\t\t.value(\"{value}\", {fullyQualifiedName}::{value})\n",
                    fmt::arg("fullyQualifiedName", m_fullyQualifiedName),
                    fmt::arg("value", value));
  }

  // Remove the last newline
  out.pop_back();
  out.push_back(';');
  out.push_back('\n');

  return out;
}

std::string Enum::getPreJS(std::string const& namePrefix,
                           std::vector<std::string>& namesToDelete) const {
  // Will no longer need the old name
  namesToDelete.push_back(createName(namePrefix));

  // Renaming the function
  // Expects to be injected where necessary
  return fmt::format("\t\t\t{baseName}: Module['{globalName}'],\n",
                     fmt::arg("baseName", m_name),
                     fmt::arg("globalName", namesToDelete.back()));
}

void Enum::addValue(std::string const& value) {
  m_values.push_back(value);
}

std::string Enum::createName(std::string const& namePrefix) const {
  return namePrefix + m_name;
}

std::string Enum::getName() const {
  return m_name;
}

}    // namespace Embind::Proxy
