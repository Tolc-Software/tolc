#include "Builders/commonBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace Builders {

std::optional<IRProxy::Structure>
getProxyStructure(clang::DeclContext const* decl) {
	if (decl->isNamespace()) {
		return IRProxy::Structure::Namespace;
	} else if (decl->isRecord()) {
		return IRProxy::Structure::Struct;
	} else if (decl->isFunctionOrMethod()) {
		return IRProxy::Structure::Function;
	}
	return {};
}

std::deque<std::pair<std::string, IRProxy::Structure>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names) {
	std::deque<std::pair<std::string, IRProxy::Structure>> structure;
	while (parent && !names.empty()) {
		auto name = names.back();
		if (auto proxyStructure = getProxyStructure(parent)) {
			// NOTE: Since we are building from the bottom up,
			//       we take from the back and push to the front
			structure.push_front({name, proxyStructure.value()});
		}
		// TODO: Handle not being able to find the correct structure
		parent = parent->getParent();
		names.pop_back();
	}
	return structure;
}

std::optional<IRProxy::AccessModifier>
convertToIRAccess(clang::AccessSpecifier access) {
	switch (access) {
		case clang::AS_public: return IRProxy::AccessModifier::Public;
		case clang::AS_private: return IRProxy::AccessModifier::Private;
		case clang::AS_protected: return IRProxy::AccessModifier::Protected;
		case clang::AS_none: return {};
	}
	return {};
}

}    // namespace Builders

