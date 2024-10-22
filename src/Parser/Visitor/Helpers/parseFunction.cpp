#include "Visitor/Helpers/parseFunction.hpp"
#include "Builders/functionBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include <clang/AST/Decl.h>
#include <functional>
#include <optional>
#include <spdlog/spdlog.h>

namespace Visitor::Helpers {
std::optional<IRProxy::Function>
parseFunction(clang::FunctionDecl* functionDecl,
              std::function<clang::QualType(clang::QualType)>
                  getPotentiallyTemplatedType) {
	auto [status, parsedFunc] =
	    Builders::buildFunction(functionDecl, getPotentiallyTemplatedType);

	using Builders::FunctionError;
	switch (status) {
		case (FunctionError::Ok): return parsedFunc.value(); break;
		case (FunctionError::ArgumentType):
			spdlog::error(R"(Failed to parse argument type for function "{}")",
			              functionDecl->getQualifiedNameAsString());
			break;
		case (FunctionError::ReturnType):
			spdlog::error(
			    R"(Failed to parse return type "{}" for function "{}")",
			    functionDecl->getReturnType().getAsString(),
			    functionDecl->getQualifiedNameAsString());
			break;
		case (FunctionError::UnsupportedOperator):
			spdlog::error(R"(Unsupported operator "{}")",
			              functionDecl->getQualifiedNameAsString());
			break;
	}
	return std::nullopt;
}
}    // namespace Visitor::Helpers
