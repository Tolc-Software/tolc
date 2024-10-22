#pragma once

#include "Visitor/ParserVisitor.hpp"
#include <IR/ir.hpp>
#include <clang/AST/ASTConsumer.h>

namespace Consumer {

class ParserConsumer : public clang::ASTConsumer {
public:
	explicit ParserConsumer(clang::ASTContext* context,
	                        IR::Namespace& parsedNamespaces,
	                        bool& parsedSuccessfully)
	    : m_visitor(context, parsedNamespaces, parsedSuccessfully) {}

	virtual void HandleTranslationUnit(clang::ASTContext& context);

private:
	Visitor::ParserVisitor m_visitor;
};

}    // namespace Consumer
