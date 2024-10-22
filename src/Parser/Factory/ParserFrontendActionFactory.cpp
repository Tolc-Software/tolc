#include "Factory/ParserFrontendActionFactory.hpp"
#include "Frontend/ParserFrontendAction.hpp"
#include <IR/ir.hpp>
#include <clang/Tooling/Tooling.h>
#include <memory>

namespace Factory {
// Used to please the ClangTool interface
// Creates a Frontend::ParserFrontendAction
// that can be used with ClangTool to parse files
std::unique_ptr<clang::tooling::FrontendActionFactory>
newParserFrontendActionFactory(IR::Namespace& ns, bool& parsed) {
	class ParserFrontendActionFactory
	    : public clang::tooling::FrontendActionFactory {
	public:
		ParserFrontendActionFactory(IR::Namespace& parsedNamespaces,
		                            bool& parsedSuccessfully)
		    : m_parsedNamespaces(parsedNamespaces),
		      m_parsedSuccessfully(parsedSuccessfully) {}
		// Gets filled with namespaces
		IR::Namespace& m_parsedNamespaces;
		// Error checking
		bool& m_parsedSuccessfully;

		std::unique_ptr<clang::FrontendAction> create() override {
			return std::make_unique<Frontend::ParserFrontendAction>(
			    m_parsedNamespaces, m_parsedSuccessfully);
		}
	};

	return std::unique_ptr<clang::tooling::FrontendActionFactory>(
	    new ParserFrontendActionFactory(ns, parsed));
}
}    // namespace Factory
