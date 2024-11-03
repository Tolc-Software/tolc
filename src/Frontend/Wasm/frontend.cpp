#include "Frontend/Wasm/frontend.hpp"
#include "Embind/Builders/moduleFileBuilder.hpp"
#include "Embind/Proxy/moduleFile.hpp"

#include <IR/ir.hpp>

#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace Frontend::Wasm {

std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace,
             std::string const& moduleName) {
  if (auto maybeModuleFile =
          Embind::Builders::buildModuleFile(rootNamespace, moduleName)) {
    auto& moduleFile = maybeModuleFile.value();

    return std::vector {
        std::make_pair(moduleFile.getCppFilepath(), moduleFile.getEmbind()),
        std::make_pair(moduleFile.getPreJSFilepath(), moduleFile.getPreJS())};
  }
  return std::nullopt;
}

}    // namespace Frontend::Wasm
