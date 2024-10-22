#include "Builders/structBuilder.hpp"
#include "Helpers/getStructData.hpp"
#include "Helpers/walkIRStructure.hpp"
#include "IR/ir.hpp"
#include <algorithm>
#include <clang/AST/Type.h>
#include <clang/Basic/Specifiers.h>
#include <deque>
#include <string>
#include <variant>

namespace {

void addEnumToVariant(std::optional<IRProxy::AccessModifier> modifier,
                      std::variant<IR::Namespace*, IR::Struct*> const& v,
                      IR::Enum e) {
	if (auto ns = std::get_if<IR::Namespace*>(&v)) {
		(*ns)->m_enums.push_back(e);
	} else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
		if (auto data = Helpers::getStructDataBasedOnAccess(**irStruct,
		                                                    modifier.value())) {
			data->m_enums.push_back(e);
		}
	}
}

IR::Enum createEnum(std::string_view name, IRProxy::Enum const& proxyEnum) {
	IR::Enum e;
	e.m_name = name;
	e.m_id = proxyEnum.m_id;

	// Representation is the fully qualified name
	e.m_representation = proxyEnum.m_fullyQualifiedName;
	e.m_documentation = proxyEnum.m_documentation;
	e.m_isScoped = proxyEnum.m_isScoped;
	e.m_values = proxyEnum.m_values;
	return e;
}

void addEnum(IRProxy::Enum const& e, IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto path = e.m_path;
	auto [name, t] = path.back();
	path.pop_back();

	// Find the parent
	auto parentOfNewFunction =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the function
	addEnumToVariant(e.m_modifier, parentOfNewFunction, createEnum(name, e));
}
}    // namespace

namespace Builders {

void buildEnums(const std::vector<IRProxy::Enum>& enums,
                IR::Namespace& globalNamespace) {
	for (auto const& e : enums) {
		addEnum(e, globalNamespace);
	}
}

}    // namespace Builders
