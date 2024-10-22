#pragma once
#include "Frontend/ParserFrontendAction.hpp"
#include <IR/ir.hpp>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace Factory {
// Used to please the ClangTool interface
// Creates a Frontend::ParserFrontendAction
// that can be used with ClangTool to parse files
std::unique_ptr<clang::tooling::FrontendActionFactory>
newParserFrontendActionFactory(IR::Namespace& ns, bool& parsed);
}    // namespace Factory
