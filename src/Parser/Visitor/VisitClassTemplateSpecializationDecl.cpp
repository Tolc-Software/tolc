#include "Builders/commonBuilder.hpp"
#include "Builders/fieldBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/Helpers/addId.hpp"
#include "Visitor/Helpers/parseFunction.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <fmt/format.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

namespace {

struct TemplateHelper {
	//     {Type name, Index in templateArgs}
	// Ex: {"int",     0}
	std::map<std::string, std::size_t> templateToTypeIndex;
	//     {Actual clang types}
	// Ex: {int}
	std::vector<clang::QualType> templateArgs;
};

// Returns the exchanged templated type if defaultType is a template parameter (say "T")
// Returns the defaultType if it is a normal type
clang::QualType getTemplatedType(TemplateHelper const& helper,
                                 clang::QualType defaultType) {
	// Is this a templated type?
	if (auto typeIndex =
	        helper.templateToTypeIndex.find(defaultType.getAsString());
	    typeIndex != helper.templateToTypeIndex.end()) {
		defaultType = helper.templateArgs[typeIndex->second];
	}
	return defaultType;
}

void adjustWithTemplateParameters(IRProxy::Function& f,
                                  IRProxy::Struct const& s) {
	// Adjust the path with the templated parent
	auto functionName = f.m_path.back();
	auto funcPath = s.m_path;
	funcPath.push_back(functionName);
	f.m_path = funcPath;

	// Adjust the fully qualified name
	f.m_fullyQualifiedName = s.m_fullyQualifiedName + "::" + functionName.first;
}

void adjustConstructorName(IRProxy::Function& f, IRProxy::Struct const& s) {
	// MyClass<int>
	auto& classInstantiation = s.m_path.back().first;
	// MyClass
	auto constructorFunction =
	    classInstantiation.substr(0, classInstantiation.find("<"));

	// Adjust the path with the templated parent
	f.m_path.back().first = constructorFunction;

	// Adjust the fully qualified name
	f.m_fullyQualifiedName =
	    s.m_fullyQualifiedName + "::" + constructorFunction;
}
}    // namespace

namespace Visitor {

bool ParserVisitor::VisitClassTemplateSpecializationDecl(
    clang::ClassTemplateSpecializationDecl* classDecl) {
	if (isInSystemHeader(classDecl) ||
	    !classDecl->isThisDeclarationADefinition()) {
		// Continue the AST search
		return true;
	}

	// Build the final parsedStruct
	IRProxy::Struct parsedStruct;

	parsedStruct.m_fullyQualifiedName = classDecl->getQualifiedNameAsString();

	// This is passed so that while extracting text from types it is exactly what the user wrote
	auto policy =
	    clang::PrintingPolicy(classDecl->getASTContext().getLangOpts());

	TemplateHelper helper;
	std::vector<std::string> templateStringArgs;
	for (auto const& templateArg : classDecl->getTemplateArgs().asArray()) {
		auto arg = templateArg.getAsType();
		if (auto type = Builders::buildType(arg, policy)) {
			parsedStruct.m_templateArguments.push_back(type.value());
		}
		helper.templateArgs.push_back(arg);
		templateStringArgs.push_back(arg.getAsString());
	}
	auto templateParameters =
	    fmt::format("<{}>", fmt::join(templateStringArgs, ", "));
	parsedStruct.m_fullyQualifiedName += templateParameters;

	parsedStruct.m_path =
	    Builders::buildStructure(classDecl, IRProxy::Structure::Struct);
	parsedStruct.m_path.back().first += templateParameters;

	Helpers::addIdToClass(parsedStruct, m_irData);

	spdlog::debug(R"(Parsing template instantiated class/struct: "{}")",
	              parsedStruct.m_fullyQualifiedName);

	if (auto classTemplate =
	        m_templateCache.findStruct(classDecl->getNameAsString())) {
		parsedStruct.m_hasImplicitDefaultConstructor =
		    classTemplate->hasDefaultConstructor();

		std::size_t i = 0;
		for (auto const& par : classTemplate->getDescribedClassTemplate()
		                           ->getTemplateParameters()
		                           ->asArray()) {
			helper.templateToTypeIndex[par->getNameAsString()] = i;
			i++;
		}

		for (auto const& memberVariable : classTemplate->fields()) {
			auto memberType =
			    getTemplatedType(helper, memberVariable->getType());
			if (auto maybeField =
			        Builders::buildField(memberVariable, memberType)) {
				auto [access, variable] = maybeField.value();
				Helpers::addIdToVariable(variable, m_irData);

				using IRProxy::AccessModifier;
				switch (access) {
					case AccessModifier::Public:
						parsedStruct.m_public.m_variables.push_back(variable);
						break;
					case AccessModifier::Private:
						parsedStruct.m_private.m_variables.push_back(variable);
						break;
					case AccessModifier::Protected:
						parsedStruct.m_protected.m_variables.push_back(
						    variable);
						break;
				}
			} else {
				spdlog::error(R"(Failed to parse member variable "{}::{}")",
				              parsedStruct.m_fullyQualifiedName,
				              memberVariable->getQualifiedNameAsString());
				m_parsedSuccessfully = false;
				// Stop parsing
				return false;
			}
		}

		for (auto const& method : classTemplate->methods()) {
			if (auto parsedFunc = Visitor::Helpers::parseFunction(
			        method, [&helper](auto type) {
				        return getTemplatedType(helper, type);
			        })) {
				auto& f = parsedFunc.value();
				adjustWithTemplateParameters(f, parsedStruct);
				if (llvm::isa<clang::CXXConstructorDecl>(method)) {
					adjustConstructorName(f, parsedStruct);
				}
				Helpers::addIdToFunction(f, m_irData);
				m_irData.m_functions.push_back(f);
			} else {
				m_parsedSuccessfully = false;
				// Stop parsing
				return false;
			}
		}

	} else {
		spdlog::error(
		    R"(Failed to find template declaration for template specialization "{}". Did it appear before the specialization in the source?)",
		    parsedStruct.m_fullyQualifiedName);
		m_parsedSuccessfully = false;
		// Stop parsing
		return false;
	}

	m_irData.m_structs.push_back(parsedStruct);
	// Continue the AST search
	return true;
}

}    // namespace Visitor
