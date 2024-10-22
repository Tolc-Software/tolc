#include "Visitor/Helpers/getDocumentation.hpp"

#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/RawCommentList.h>
#include <clang/Basic/CommentOptions.h>
#include <clang/Basic/Diagnostic.h>
#include <string>

namespace Visitor::Helpers {

std::string getDocumentation(clang::Decl* decl) {
	std::string documentation;
	auto& context = decl->getASTContext();

	if (const auto* rawComment = context.getRawCommentForDeclNoCache(decl)) {
		clang::DiagnosticsEngine diags(new clang::DiagnosticIDs,
		                               new clang::DiagnosticOptions);
		documentation =
		    rawComment->getFormattedText(context.getSourceManager(), diags);
	}
	return documentation;
}
}    // namespace Visitor::Helpers
