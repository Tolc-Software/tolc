#include "Builders/commonBuilder.hpp"
#include "Builders/fieldBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/utilities.hpp"
#include "Helpers/Utils/split.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/Helpers/addId.hpp"
#include "Visitor/Helpers/getDocumentation.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <clang/AST/Decl.h>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

namespace Visitor {

std::optional<IR::Variable>
buildVar(clang::VarDecl* field,
         IRProxy::IRData& data,
         std::optional<clang::QualType> templateSpecialization = std::nullopt) {
	IR::Variable variable;
	variable.m_name = field->getName();
	variable.m_documentation = Visitor::Helpers::getDocumentation(field);

	// This is passed so that while extracting text from types it is exactly what the user wrote
	auto policy = clang::PrintingPolicy(field->getASTContext().getLangOpts());

	if (auto type = Builders::buildType(templateSpecialization ?
                                            templateSpecialization.value() :
                                            field->getType(),
	                                    policy)) {
		variable.m_type = type.value();
	} else {
		return std::nullopt;
	}

	Helpers::addIdToVariable(variable, data);

	return variable;
}

std::string getParent(std::string const& name) {
	auto splitNames = ::Helpers::Utils::split(name, "::");
	splitNames.pop_back();
	return fmt::format("{}", fmt::join(splitNames, "::"));
}

bool ParserVisitor::VisitVarDecl(clang::VarDecl* varDecl) {
	// Don't care about varDecl that is inside a locally defined function body
	if (isInSystemHeader(varDecl) || varDecl->isLocalVarDeclOrParm()) {
		// Continue the AST search
		return true;
	}
	spdlog::debug(R"(Parsing member variable: "{}")",
	              varDecl->getQualifiedNameAsString());

	if (auto maybeVariable = buildVar(varDecl, m_irData)) {
		IR::Variable variable = maybeVariable.value();

		switch (varDecl->getStorageClass()) {
			case (clang::StorageClass::SC_Static):
				variable.m_isStatic = true;
				break;
			case (clang::StorageClass::SC_None):
			case (clang::StorageClass::SC_Extern):
			case (clang::StorageClass::SC_PrivateExtern):
			case (clang::StorageClass::SC_Auto):
			case (clang::StorageClass::SC_Register):
				variable.m_isStatic = false;
				break;
		}

		// Add so it can be put into the IR AST later
		auto parentName = getParent(varDecl->getQualifiedNameAsString());
		if (varDecl->isStaticDataMember()) {
			IRProxy::AccessModifier modifier;
			if (auto accessModifier =
			        Builders::convertToIRAccess(varDecl->getAccess())) {
				modifier = accessModifier.value();
			} else {
				spdlog::error(R"(Failed to parse member variable "{}")",
				              varDecl->getQualifiedNameAsString());
				m_parsedSuccessfully = false;
				// Stop parsing
				return false;
			}
			// FieldDecl are for non-static members
			IRProxy::MemberVariable m;
			m.m_variable = variable;
			m.m_modifier = modifier;
			m_irData.m_memberVariables[parentName].push_back(m);
		} else {
			m_irData.m_globalVariables[parentName].push_back(variable);
		}

	} else {
		spdlog::error(R"(Failed to parse member variable "{}")",
		              varDecl->getQualifiedNameAsString());
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	// Continue the AST search
	return true;
}
}    // namespace Visitor
