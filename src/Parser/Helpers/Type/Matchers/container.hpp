#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string_view>

namespace Helpers::Type::Matchers {

// Tries to match a container type to the string and return it
// Base types: Vector, Map, ...
std::optional<IR::ContainerType> getContainerType(std::string_view type);
}
