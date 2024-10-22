#include "Consumer/ParserConsumer.hpp"
#include "Visitor/ParserVisitor.hpp"
#include "clang/AST/ASTConsumer.h"

namespace Consumer {

void ParserConsumer::HandleTranslationUnit(clang::ASTContext& context) {
	m_visitor.TraverseDecl(context.getTranslationUnitDecl());
}

}    // namespace Consumer
