#pragma once

#include <IR/ir.hpp>

#include <string>

namespace TestUtil {

IR::Namespace parseString(std::string const& code);

IR::Namespace parseFile(std::string const& filepath);
}    // namespace TestUtil
