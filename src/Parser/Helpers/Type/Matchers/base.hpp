#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {
std::optional<IR::BaseType> getBaseType(std::string_view type);
}
