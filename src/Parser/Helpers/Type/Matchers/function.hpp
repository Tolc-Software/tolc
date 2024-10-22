#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {
std::optional<IR::Type::Function> getFunctionType(std::string_view type);
}
