#pragma once

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <algorithm>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace TestUtil {

enum class AccessModifier { Public, Private, Protected };

IR::StructData const*
getStructDataBasedOnAccess(IR::Struct const& s,
                           TestUtil::AccessModifier access) {
  using TestUtil::AccessModifier;
  switch (access) {
    case AccessModifier::Public: return &s.m_public;
    case AccessModifier::Private: return &s.m_private;
    case AccessModifier::Protected: return &s.m_protected;
  }
  return nullptr;
}

IR::Variable& findVariable(IR::Namespace& parent, std::string const& name) {
  auto& variables = parent.m_variables;
  REQUIRE(variables.size() >= 1);
  auto v = std::find_if(variables.begin(),
                        variables.end(),
                        [&name](auto const& n) { return n.m_name == name; });

  REQUIRE(v != variables.end());
  return *v;
}

IR::Enum& findEnum(IR::Namespace& parent, std::string const& name) {
  auto& enums = parent.m_enums;
  REQUIRE(enums.size() >= 1);
  auto v = std::find_if(enums.begin(), enums.end(), [&name](auto const& n) {
    return n.m_name == name;
  });

  REQUIRE(v != enums.end());
  return *v;
}

IR::Enum const& findEnum(IR::Struct const& parent,
                         std::string const& name,
                         TestUtil::AccessModifier access) {
  auto data = getStructDataBasedOnAccess(parent, access);
  REQUIRE(data != nullptr);
  auto& enums = data->m_enums;
  REQUIRE(enums.size() >= 1);
  auto v = std::find_if(enums.begin(), enums.end(), [&name](auto const& n) {
    return n.m_name == name;
  });

  REQUIRE(v != enums.end());
  return *v;
}

IR::Namespace& findNamespace(IR::Namespace& parent, std::string const& name) {
  auto& namespaces = parent.m_namespaces;
  REQUIRE(namespaces.size() >= 1);
  auto ns = std::find_if(namespaces.begin(),
                         namespaces.end(),
                         [&name](auto const& n) { return n.m_name == name; });

  REQUIRE(ns != namespaces.end());
  return *ns;
}

IR::Function& findFunction(IR::Namespace& parent, std::string const& name) {
  auto& functions = parent.m_functions;
  REQUIRE(functions.size() >= 1);
  auto function =
      std::find_if(functions.begin(), functions.end(), [&name](auto const& f) {
        return f.m_name == name;
      });

  REQUIRE(function != functions.end());
  return *function;
}

IR::Argument& findArgument(IR::Function& f, std::string const& name) {
  auto& arguments = f.m_arguments;
  REQUIRE(arguments.size() >= 1);
  auto argument =
      std::find_if(arguments.begin(), arguments.end(), [&name](auto const& a) {
        return a.m_name == name;
      });

  REQUIRE(argument != arguments.end());
  return *argument;
}

IR::Function const& findFunction(IR::Struct& parent,
                                 std::string const& name,
                                 TestUtil::AccessModifier access) {
  auto data = getStructDataBasedOnAccess(parent, access);
  REQUIRE(data != nullptr);
  auto& functions = data->m_functions;
  REQUIRE(functions.size() >= 1);
  auto function =
      std::find_if(functions.begin(), functions.end(), [&name](auto const& f) {
        return f.m_name == name;
      });

  REQUIRE(function != functions.end());
  return *function;
}

IR::Variable const& findMember(IR::Struct& parent,
                               std::string const& name,
                               TestUtil::AccessModifier access) {
  auto data = getStructDataBasedOnAccess(parent, access);
  REQUIRE(data != nullptr);
  auto& members = data->m_memberVariables;
  REQUIRE(members.size() >= 1);
  auto member =
      std::find_if(members.begin(), members.end(), [&name](auto const& m) {
        return m.m_name == name;
      });

  REQUIRE(member != members.end());
  return *member;
}

IR::Struct& findStruct(IR::Namespace& parent,
                       std::string const& name,
                       bool representation = false) {
  auto& structs = parent.m_structs;
  REQUIRE(structs.size() >= 1);
  auto strct = std::find_if(
      structs.begin(), structs.end(), [&name, representation](auto const& s) {
        return representation ? s.m_representation == name : s.m_name == name;
      });

  REQUIRE(strct != structs.end());
  return *strct;
}

template <typename IRType>
std::optional<IRType> findWithName(std::string_view name,
                                   std::vector<IRType> const& v) {
  if (auto it =
          std::find_if(v.begin(),
                       v.end(),
                       [&name](auto const& ir) { return ir.m_name == name; });
      it != v.end()) {
    return *it;
  }
  return {};
}

}    // namespace TestUtil
