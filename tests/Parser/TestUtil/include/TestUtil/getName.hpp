#pragma once

#include "IRProxy/IRData.hpp"

#include <IR/ir.hpp>
#include <catch2/catch_test_macros.hpp>

#include <string>
#include <utility>

namespace TestUtil {
std::pair<std::string, std::string>
getNameAndRepresentation(IRProxy::Structure const& structure,
                         IR::Namespace const& ns) {
  using IRProxy::Structure;
  switch (structure) {
    case Structure::Namespace:
      REQUIRE(ns.m_namespaces.size() == 1);
      return {ns.m_namespaces[0].m_name, ns.m_namespaces[0].m_representation};
      break;
    case Structure::Struct:
      REQUIRE(ns.m_structs.size() == 1);
      return {ns.m_structs[0].m_name, ns.m_structs[0].m_representation};
      break;
    case Structure::Function:
      REQUIRE(ns.m_functions.size() == 1);
      return {ns.m_functions[0].m_name, ns.m_functions[0].m_representation};
      break;
    case Structure::Enum: break;
  }
  return {"default name", "default structure"};
}
}    // namespace TestUtil
