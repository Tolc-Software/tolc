#pragma once

#include "IRProxy/IRData.hpp"
#include "clang/AST/Decl.h"
#include <functional>

namespace Visitor::Helpers {
std::optional<IRProxy::Function> parseFunction(
    clang::FunctionDecl* functionDecl,
    std::function<clang::QualType(clang::QualType)>
        getPotentiallyTemplatedType = [](auto t) { return t; });
}    // namespace Visitor::Helpers
