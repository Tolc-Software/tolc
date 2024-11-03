#include "Embind/Builders/moduleFileBuilder.hpp"
#include "Embind/Builders/moduleBuilder.hpp"
#include "Embind/Helpers/combine.hpp"
#include "Embind/Proxy/moduleFile.hpp"
#include "Embind/Proxy/typeInfo.hpp"

#include <IR/ir.hpp>

#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
  IR::Namespace const& m_namespace;
  Embind::Proxy::Module m_module;
};

// Builds the submodules corresponding to the sub namespaces of currentNamespace
// Adds each built submodule to namespaces
// Returns false on failure
bool tryBuildSubModules(IR::Namespace const& currentNamespace,
                        Embind::Proxy::TypeInfo& typeInfo,
                        std::queue<ModulePair>& namespaces) {
  for (auto const& subNamespace : currentNamespace.m_namespaces) {
    if (auto m = Embind::Builders::buildModule(subNamespace, typeInfo)) {
      namespaces.push({subNamespace, m.value()});
    } else {
      return false;
    }
  }
  return true;
}

}    // namespace

namespace Embind::Builders {

std::optional<Embind::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
  Embind::Proxy::TypeInfo typeInfo;

  if (auto maybeRootModule =
          Embind::Builders::buildModule(rootNamespace, typeInfo)) {
    auto rootModule = maybeRootModule.value();
    Embind::Proxy::ModuleFile moduleFile(rootModule, rootModuleName);

    std::queue<ModulePair> namespaces;
    if (!tryBuildSubModules(rootNamespace, typeInfo, namespaces)) {
      return std::nullopt;
    }

    while (!namespaces.empty()) {
      auto const& [currentNamespace, currentModule] = namespaces.front();

      moduleFile.addModule(currentModule);

      // Go deeper into the nested namespaces
      if (!tryBuildSubModules(currentNamespace, typeInfo, namespaces)) {
        return std::nullopt;
      }

      // Need currentNamespace and currentModule to live this far
      namespaces.pop();
    }

    moduleFile.setTypeInfo(typeInfo);
    return moduleFile;
  }

  return std::nullopt;
}
}    // namespace Embind::Builders
