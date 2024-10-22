#pragma once

#include "IRProxy/IRData.hpp"
#include "Template/cache.hpp"
#include <IR/ir.hpp>
#include <clang/AST/RecursiveASTVisitor.h>
#include <clang/Basic/SourceManager.h>
#include <vector>

namespace Visitor {
class ParserVisitor : public clang::RecursiveASTVisitor<ParserVisitor> {
public:
	explicit ParserVisitor(clang::ASTContext* context,
	                       IR::Namespace& parsedNamespaces,
	                       bool& parsedSuccessfully);

	// Will populate the m_parsedNamespaces
	// since it is guaranteed that all visitors will have run by then
	~ParserVisitor();

	bool VisitFunctionDecl(clang::FunctionDecl* functionDecl);

	bool VisitFunctionTemplateDecl(clang::FunctionTemplateDecl* functionDecl);

	bool VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl);

	bool VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl);

	bool VisitClassTemplateSpecializationDecl(
	    clang::ClassTemplateSpecializationDecl* classDecl);

	// Member variables
	bool VisitFieldDecl(clang::FieldDecl* fieldDecl);

	// Static member variables
	bool VisitVarDecl(clang::VarDecl* varDecl);

	bool VisitEnumDecl(clang::EnumDecl* enumDecl);

	// Return whether this visitor should recurse into
	// template instantiations.
	// Determines behaviour of the ASTVisitor
	bool shouldVisitTemplateInstantiations() const;

private:
	clang::ASTContext* m_context;

	// Acts as a cache to build the IR
	IRProxy::IRData m_irData;

	// Used as a lookup for template instantiations
	// NOTE: Requires templates to be specified before instantiations in the code
	Template::Cache m_templateCache;

	// The final IR AST
	// This will get filled in the destructor of the visitor
	IR::Namespace& m_parsedNamespaces;

	// Set to false if any visitor found an unrecoverable error
	bool& m_parsedSuccessfully;

	// Returns true if either from the standard library or from a header marked as System (i.e. not searcheable for errors)
	bool isInSystemHeader(clang::Decl* decl);

	// Returns true iff functionDecl is a template (false if a specialization)
	bool isPureTemplate(clang::FunctionDecl* functionDecl);

	// Returns true iff fieldDecl is a template (false if a specialization)
	bool isPureTemplate(clang::FieldDecl* fieldDecl);

	// Returns true iff classDecl is a template (false if a specialization)
	bool isPureTemplate(clang::CXXRecordDecl* classDecl);

	bool isTemplateSpecialization(clang::CXXRecordDecl* classDecl);
};
}    // namespace Visitor
