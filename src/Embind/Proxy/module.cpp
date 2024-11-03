#include "Embind/Proxy/module.hpp"
#include "Embind/Helpers/split.hpp"
#include "Embind/joinCalls.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <deque>
#include <numeric>
#include <string>
#include <vector>

namespace Embind::Proxy {

std::string Module::getEmbind() const {
  using namespace Embind;
  return fmt::format(
      "{functions}{classes}{enums}{attributes}",
      fmt::arg("functions", joinEmbind(m_namePrefix, m_functions)),
      fmt::arg("classes", joinEmbind(m_namePrefix, m_classes)),
      fmt::arg("enums", joinEmbind(m_namePrefix, m_enums)),
      fmt::arg("attributes", joinEmbind(m_namePrefix, m_attributes)));
}

namespace {
// Return a unique prefix name that can be used in the generated code for this module
std::string createPrefix(std::deque<std::string> path) {
  // MyNS::Math -> MyNs_Math
  // This is to avoid naming conflicts when defining functions/classes with the
  // same name in different namespaces

  // If qualifiedName is the root name (global namespace has no name)
  // This will return ""
  auto prefix = fmt::format("{}", fmt::join(path, "_"));
  return prefix.empty() ? prefix : prefix + '_';
}

std::string getModuleDeclaration(std::deque<std::string> const& path) {
  std::string decl = "Module";
  for (auto const& p : path) {
    decl += fmt::format("['{}']", p);
  }

  return decl += " =";
}

std::string joinDeletions(std::vector<std::string> const& deletions) {
  std::string joined;
  for (auto const& d : deletions) {
    joined += fmt::format("\t\tdelete Module['{}'];\n", d);
  }
  return joined;
}

}    // namespace

std::string Module::getPreJS() const {
  std::vector<std::string> namesToDelete;

  // Move things into classes that are not supported in Embind
  std::string preJS =
      Embind::joinGlobalPreJS(m_namePrefix, m_classes, namesToDelete);

  if (m_path.empty()) {
    // The rest are renaming things into their namespace
    // For the global there are no renames
    return preJS + "\n" + joinDeletions(namesToDelete);
  }

  using namespace Embind;
  preJS += fmt::format(
      R"(
		{moduleDeclaration} {{
{functions}{enums}{attributes}{classes}		}};
)",
      fmt::arg("moduleDeclaration", getModuleDeclaration(m_path)),
      fmt::arg("functions",
               joinPreJS(m_namePrefix, m_functions, namesToDelete)),
      fmt::arg("enums", joinPreJS(m_namePrefix, m_enums, namesToDelete)),
      fmt::arg("attributes",
               joinPreJS(m_namePrefix, m_attributes, namesToDelete)),
      fmt::arg("classes", joinPreJS(m_namePrefix, m_classes, namesToDelete)));

  // Remove all the previous names
  preJS += joinDeletions(namesToDelete);

  return preJS;
}

Module::Module(std::string const& name, std::string const& qualifiedName)
  : m_name(name), m_path(Embind::Helpers::split(qualifiedName, "::")),
    m_namePrefix(), m_functions(), m_enums(), m_attributes() {
  m_namePrefix = createPrefix(m_path);
}

void Module::addFunction(Function const& function) {
  m_functions.push_back(function);
}

void Module::addClass(Class const& c) {
  m_classes.push_back(c);
}

void Module::addEnum(Enum const& e) {
  m_enums.push_back(e);
}

void Module::addAttribute(Attribute const& a) {
  m_attributes.push_back(a);
}

std::string const& Module::getPrefix() const {
  return m_namePrefix;
}

}    // namespace Embind::Proxy
