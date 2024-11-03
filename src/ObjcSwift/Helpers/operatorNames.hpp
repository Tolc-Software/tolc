#pragma once

#include <IR/ir.hpp>
#include <optional>
#include <string>

namespace ObjcSwift::Helpers {

std::optional<std::string> getOperatorName(IR::Operator op);
}
