#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <clang/AST/Decl.h>
#include <optional>
#include <vector>

namespace Builders {

/**
  * The type of errors that can be had while parsing a clang::FunctionDecl to a IRProxy::Function
  */
enum class FunctionError { Ok, ReturnType, ArgumentType, UnsupportedOperator };

/**
  * Convert a clang::FunctionDecl to IRProxy::Function
  * Optionally provide a function that can resolve templated types (defaults to doing nothing)
  */
std::pair<FunctionError, std::optional<IRProxy::Function>> buildFunction(
    clang::FunctionDecl* functionDecl,
    std::function<clang::QualType(clang::QualType)>
        getPotentiallyTemplatedType = [](auto t) { return t; });

/**
  * Convert the IRProxy::Function to IR::Function and add them to the structure
  */
void buildFunctions(const std::vector<IRProxy::Function>& functions,
                    IR::Namespace& globalNamespace);

}    // namespace Builders
