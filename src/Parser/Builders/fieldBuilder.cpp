#include "Builders/fieldBuilder.hpp"
#include "Builders/commonBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/Helpers/getDocumentation.hpp"
#include <IR/ir.hpp>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>
#include <optional>

namespace Builders {
std::optional<std::pair<IRProxy::AccessModifier, IR::Variable>>
buildField(clang::FieldDecl* field,
           std::optional<clang::QualType> templateSpecialization) {
	IR::Variable variable;
	IRProxy::AccessModifier modifier;
	variable.m_name = field->getName();

	// This is passed so that while extracting text from types it is exactly what the user wrote
	auto policy = clang::PrintingPolicy(field->getASTContext().getLangOpts());

	if (auto type = Builders::buildType(templateSpecialization ?
                                            templateSpecialization.value() :
                                            field->getType(),
	                                    policy)) {
		variable.m_type = type.value();
		// Static members are of type VarDecl
		variable.m_isStatic = false;

		variable.m_documentation = Visitor::Helpers::getDocumentation(field);
	} else {
		return std::nullopt;
	}

	if (auto accessModifier = Builders::convertToIRAccess(field->getAccess())) {
		modifier = accessModifier.value();
	} else {
		return std::nullopt;
	}

	return std::make_pair(modifier, variable);
}
}    // namespace Builders
