#pragma once

#include "Embind/Proxy/attribute.hpp"
#include "Embind/Proxy/class.hpp"
#include "Embind/Proxy/enum.hpp"
#include "Embind/Proxy/function.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <deque>
#include <string>
#include <vector>

namespace Embind::Proxy {

struct Module {
  explicit Module(std::string const& name, std::string const& qualifiedName);

  void addFunction(Function const& function);

  void addClass(Class const& c);

  void addEnum(Enum const& e);

  void addAttribute(Attribute const& a);

  std::string const& getPrefix() const;

  std::string getEmbind() const;

  std::string getPreJS() const;

  private:
  // The name of the associated namespace
  std::string m_name;

  // Path to this module
  // E.g.
  // {FirstModule, NestedModule, ThisModule}
  std::deque<std::string> m_path;

  // Every variable starts with this.
  // Should be "" for global modules
  // Should be "Nested_Namespace" for other modules
  // Will result in function "MyNS::func" being used from js as "MyNS_func"
  std::string m_namePrefix;

  std::vector<Function> m_functions;
  std::vector<Class> m_classes;
  std::vector<Enum> m_enums;
  std::vector<Attribute> m_attributes;
};
}    // namespace Embind::Proxy
