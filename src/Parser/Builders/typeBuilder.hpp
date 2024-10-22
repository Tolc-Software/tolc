#pragma once

#include <IR/ir.hpp>
#include <clang/AST/Type.h>
#include <optional>

namespace Builders {

/**
* @brief: Tries to create an IR::Type of the input
*         If it fails it returns std::nullopt
*
* @param: std::string_view type
*         const clang::PrintingPolicy& policy - Used to get the context for the type
*
* @return: std::optional<IR::Type>
*/
std::optional<IR::Type> buildType(clang::QualType type,
                                  clang::PrintingPolicy options);

}    // namespace Builders
