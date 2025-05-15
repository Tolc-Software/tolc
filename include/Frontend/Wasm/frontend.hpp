#pragma once

#include <IR/ir.hpp>

#include <filesystem>
#include <optional>
#include <string>
#include <utility>
#include <vector>

namespace Frontend::Wasm {
/**
  * Takes a representation of some C++ code.
  * Returns a file and the corresponding file content.
  * The file content is the converted IR into embind bindings.
  */
std::optional<std::vector<std::pair<std::filesystem::path, std::string>>>
createModule(IR::Namespace const& rootNamespace, std::string const& moduleName);
}    // namespace Frontend::Wasm
