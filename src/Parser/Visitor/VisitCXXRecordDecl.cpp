#include "Builders/commonBuilder.hpp"
#include "Helpers/getStructData.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/Helpers/addId.hpp"
#include "Visitor/Helpers/getDocumentation.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>
#include <string>

namespace Visitor {

bool ParserVisitor::VisitCXXRecordDecl(clang::CXXRecordDecl* classDecl) {
	// Also filters out forward declarations
	if (isInSystemHeader(classDecl) ||
	    !classDecl->isThisDeclarationADefinition()) {
		// Continue the AST search
		return true;
	}

	if (isPureTemplate(classDecl)) {
		// Save for later parsing
		m_templateCache.m_structs.push_back(classDecl);
		// Continue the AST search
		return true;
	}
	if (isTemplateSpecialization(classDecl)) {
		// Continue the AST search
		return true;
	}

	spdlog::debug(R"(Parsing class/struct: "{}")",
	              classDecl->getQualifiedNameAsString());

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;

	parsedStruct.m_fullyQualifiedName = classDecl->getQualifiedNameAsString();

	parsedStruct.m_documentation =
	    Visitor::Helpers::getDocumentation(classDecl);

	parsedStruct.m_path =
	    Builders::buildStructure(classDecl, IRProxy::Structure::Struct);

	parsedStruct.m_hasImplicitDefaultConstructor =
	    classDecl->needsImplicitDefaultConstructor();

	parsedStruct.m_modifier =
	    Builders::convertToIRAccess(classDecl->getAccess());

	Helpers::addIdToClass(parsedStruct, m_irData);

	// Inheritence
	for (auto const& base : classDecl->bases()) {
		if (auto maybeAccess = ::Helpers::getStructDataBasedOnAccess(
		        parsedStruct, base.getAccessSpecifier())) {
			auto policy =
			    clang::PrintingPolicy(classDecl->getASTContext().getLangOpts());
			maybeAccess->m_inherited.push_back(
			    base.getType().getAsString(policy));
		}
	}

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}    // namespace Visitor
