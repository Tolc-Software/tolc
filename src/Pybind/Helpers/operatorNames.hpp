#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string>

namespace Pybind::Helpers {

std::optional<std::string> getOperatorName(IR::Operator op);
}
