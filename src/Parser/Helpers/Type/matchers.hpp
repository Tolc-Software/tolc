#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type {
// Tries to match a base type to the string and return it
// Base types: Int, Void, ...
std::optional<IR::Type::Value> getValueType(std::string_view type);

// Tries to match the input to an enum
std::optional<IR::Type::EnumValue> getEnumType(std::string_view type);

// Tries to match the input to a function type
std::optional<IR::Type::Function> getFunctionMetaType(std::string_view type);

// Tries to match the input to a user defined struct type
std::optional<IR::Type::UserDefined> getUserDefinedType(std::string_view type);
}    // namespace Helpers::Type
