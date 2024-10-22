#include "Helpers/getStructData.hpp"
#include "Builders/commonBuilder.hpp"
#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>

namespace Helpers {

IR::StructData* getStructDataBasedOnAccess(IR::Struct& s,
                                           IRProxy::AccessModifier access) {
	using IRProxy::AccessModifier;
	switch (access) {
		case AccessModifier::Public: return &s.m_public;
		case AccessModifier::Private: return &s.m_private;
		case AccessModifier::Protected: return &s.m_protected;
	}
	return nullptr;
}

IRProxy::StructData* getStructDataBasedOnAccess(IRProxy::Struct& s,
                                                clang::AccessSpecifier access) {
	if (auto maybeAccess = Builders::convertToIRAccess(access)) {
		using IRProxy::AccessModifier;
		switch (maybeAccess.value()) {
			case AccessModifier::Public: return &s.m_public;
			case AccessModifier::Private: return &s.m_private;
			case AccessModifier::Protected: return &s.m_protected;
		}
	}
	return nullptr;
}
}    // namespace Helpers
