#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string>

namespace TestUtil {
IR::Namespace parse(std::string code);

std::optional<IR::Namespace> parseUnsafe(std::string code);
}

