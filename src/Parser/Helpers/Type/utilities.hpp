#pragma once

#include <IR/ir.hpp>
#include <clang/AST/Type.h>
#include <optional>
#include <string_view>

namespace Helpers::Type {

clang::QualType getTypeWithPointersRemoved(clang::QualType type);

/**
  * Take a base and builds a type
  */
IR::Type buildTypeFromBase(IR::BaseType valueType,
                           std::optional<IR::BaseType> keyType = std::nullopt);

/**
* Tries to create an IR::Type (Int, Void, String, ...) of the input
* If it fails it returns std::nullopt
* NOTE: Assumes the qualifiers are removed (const, ...)
*/
std::optional<IR::Type> getIRType(std::string_view type);

int getNumberOfPointers(clang::QualType type);

/**
* Checks if type is const
*/
bool isConst(clang::QualType const& type);
}    // namespace Helpers::Type
