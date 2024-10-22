#pragma once

#include "IR/ir.hpp"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include <vector>

namespace Frontend {

class ParserFrontendAction : public clang::ASTFrontendAction {
public:
	explicit ParserFrontendAction(IR::Namespace& parsedNamespaces,
	                              bool& parsedSuccessfully);

	std::unique_ptr<clang::ASTConsumer>
	CreateASTConsumer(clang::CompilerInstance& compiler,
	                  llvm::StringRef /*InFile*/);

	IR::Namespace& m_parsedNamespaces;
	bool& m_parsedSuccessfully;
};

}    // namespace Frontend
