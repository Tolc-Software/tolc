#include "Builders/commonBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Visitor/Helpers/addId.hpp"
#include "Visitor/Helpers/parseFunction.hpp"
#include "Visitor/ParserVisitor.hpp"
#include <clang/AST/PrettyPrinter.h>
#include <spdlog/spdlog.h>

namespace {
void adjustWithTemplateParams(IRProxy::Function& f,
                              clang::FunctionDecl* fDecl) {
	// Potentially add the template parameters to name/representation
	std::vector<std::string> params;
	if (auto templateArgs = fDecl->getTemplateSpecializationArgs()) {
		// This is passed so that while extracting text from types it is exactly what the user wrote
		auto policy =
		    clang::PrintingPolicy(fDecl->getASTContext().getLangOpts());

		for (auto param : templateArgs->asArray()) {
			if (auto type = Builders::buildType(param.getAsType(), policy)) {
				f.m_templateArguments.push_back(type.value());
				params.push_back(type.value().m_representation);
			}
		}
		if (!params.empty()) {
			// <int, double>
			std::string paramPack =
			    fmt::format("<{}>", fmt::join(params, ", "));
			f.m_fullyQualifiedName += paramPack;
			f.m_path.back().first += paramPack;
		}
	}
}
}    // namespace

namespace Visitor {
bool ParserVisitor::VisitFunctionTemplateDecl(
    clang::FunctionTemplateDecl* functionDecl) {
	if (isInSystemHeader(functionDecl)) {
		// Continue the AST search
		return true;
	}

	spdlog::debug(R"(Parsing function: "{}")",
	              functionDecl->getQualifiedNameAsString());

	for (auto specialization : functionDecl->specializations()) {
		if (auto parsedFunc = Visitor::Helpers::parseFunction(specialization)) {
			auto& f = parsedFunc.value();
			adjustWithTemplateParams(f, specialization);
			Helpers::addIdToFunction(f, m_irData);
			m_irData.m_functions.push_back(f);
		} else {
			m_parsedSuccessfully = false;
			// Stop parsing
			return false;
		}
	}

	// Continue the AST search
	return true;
}

}    // namespace Visitor
