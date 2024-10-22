#include "Builders/commonBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/Helpers/addId.hpp"
#include "Visitor/Helpers/getDocumentation.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <spdlog/spdlog.h>

namespace Visitor {

bool ParserVisitor::VisitEnumDecl(clang::EnumDecl* enumDecl) {
	if (isInSystemHeader(enumDecl) ||
	    !enumDecl->isThisDeclarationADefinition()) {
		// Continue the AST search
		return true;
	}

	spdlog::debug(R"(Parsing enum: "{}")",
	              enumDecl->getQualifiedNameAsString());

	// Build the final parsedEnum
	IRProxy::Enum parsedEnum;

	parsedEnum.m_fullyQualifiedName = enumDecl->getQualifiedNameAsString();

	parsedEnum.m_documentation = Visitor::Helpers::getDocumentation(enumDecl);

	parsedEnum.m_path =
	    Builders::buildStructure(enumDecl, IRProxy::Structure::Enum);

	parsedEnum.m_isScoped = enumDecl->isScopedUsingClassTag();

	for (auto value : enumDecl->enumerators()) {
		parsedEnum.m_values.push_back(value->getNameAsString());
	}

	// Check for access modifiers (public, private, ...)
	parsedEnum.m_modifier = Builders::convertToIRAccess(enumDecl->getAccess());

	Helpers::addIdToEnum(parsedEnum, m_irData);

	m_irData.m_enums.push_back(parsedEnum);
	// Continue the AST search
	return true;
}
}    // namespace Visitor
