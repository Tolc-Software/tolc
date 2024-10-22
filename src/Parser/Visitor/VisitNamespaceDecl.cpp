#include "Visitor/Helpers/getDocumentation.hpp"
#include "Visitor/ParserVisitor.hpp"
#include "clang/AST/DeclBase.h"
#include "clang/AST/Type.h"
#include <spdlog/spdlog.h>

namespace Visitor {

bool ParserVisitor::VisitNamespaceDecl(clang::NamespaceDecl* namespaceDecl) {
	if (isInSystemHeader(namespaceDecl)) {
		// Continue the AST search
		return true;
	}
	spdlog::debug(R"(Parsing namespace: "{}")",
	              namespaceDecl->getQualifiedNameAsString());

	IRProxy::Namespace parsedNamespace;
	parsedNamespace.m_fullyQualifiedName =
	    namespaceDecl->getQualifiedNameAsString();

	parsedNamespace.m_documentation =
	    Visitor::Helpers::getDocumentation(namespaceDecl);

	// Export our parsed namespace
	m_irData.m_namespaces.push_back(parsedNamespace);

	// Continue the AST search
	return true;
}

}
