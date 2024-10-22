#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <clang/AST/Decl.h>
#include <clang/AST/Type.h>
#include <optional>

namespace Builders {

/**
* @brief: Tries to create an IR version of the member variable input
*         If templateSpecialization is provided, this field was part of a template and the type had to be provided
*         If it fails it returns std::nullopt
*
* @param: clang::FieldDecl field
*         std::optional<clang::QualType> templateSpecialization
*
* @return: std::optional<std::pair<IRProxy::AccessModifier modifier, IR::Variable variable>>
*/
std::optional<std::pair<IRProxy::AccessModifier, IR::Variable>> buildField(
    clang::FieldDecl* field,
    std::optional<clang::QualType> templateSpecialization = std::nullopt);

}    // namespace Builders
