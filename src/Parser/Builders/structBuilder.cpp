#include "Builders/structBuilder.hpp"
#include "Helpers/getStructData.hpp"
#include "Helpers/walkIRStructure.hpp"
#include "IR/ir.hpp"
#include <algorithm>
#include <deque>
#include <optional>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>

namespace {

void addStructToVariant(std::optional<IRProxy::AccessModifier> modifier,
                        std::variant<IR::Namespace*, IR::Struct*> const& v,
                        IR::Struct newStruct) {
	if (auto ns = std::get_if<IR::Namespace*>(&v)) {
		auto& children = (*ns)->m_structs;
		children.push_back(newStruct);
	} else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
		if (auto structData = Helpers::getStructDataBasedOnAccess(
		        **irStruct, modifier.value())) {
			auto& children = structData->m_structs;
			children.push_back(newStruct);
		}
	}
}

IR::Struct createStruct(IRProxy::Struct& s) {
	auto& [name, _structure] = s.m_path.back();

	IR::Struct newStruct;
	newStruct.m_name = name;
	newStruct.m_id = s.m_id;
	// Representation is the fully qualified name
	newStruct.m_representation = s.m_fullyQualifiedName;
	newStruct.m_documentation = s.m_documentation;

	auto& pub = newStruct.m_public;
	pub.m_memberVariables = s.m_public.m_variables;
	pub.m_inherited = s.m_public.m_inherited;

	auto& priv = newStruct.m_private;
	priv.m_memberVariables = s.m_private.m_variables;
	priv.m_inherited = s.m_private.m_inherited;

	auto& prot = newStruct.m_protected;
	prot.m_memberVariables = s.m_protected.m_variables;
	prot.m_inherited = s.m_protected.m_inherited;

	newStruct.m_templateArguments = s.m_templateArguments;

	newStruct.m_hasImplicitDefaultConstructor =
	    s.m_hasImplicitDefaultConstructor;
	return newStruct;
}

void addStruct(IRProxy::Struct& s, IR::Namespace& globalNamespace) {
	// Take out the already created path
	auto path = s.m_path;
	path.pop_back();

	// Find the parent
	auto parentOfNewStruct =
	    Helpers::walkPathThroughStructure(path, globalNamespace);

	// Create and add the struct
	addStructToVariant(s.m_modifier, parentOfNewStruct, createStruct(s));
}

std::optional<std::vector<IRProxy::MemberVariable>>
getVariables(IRProxy::Struct const& s,
             std::map<std::string, std::vector<IRProxy::MemberVariable>>&
                 memberVariables) {
	if (auto variables = memberVariables.find(s.m_fullyQualifiedName);
	    variables != memberVariables.end()) {
		return variables->second;
	}
	return {};
}

}    // namespace

namespace Builders {

void addMemberVariables(
    std::vector<IRProxy::Struct>& structs,
    std::map<std::string, std::vector<IRProxy::MemberVariable>>&
        memberVariables) {
	for (auto& s : structs) {
		if (auto variables = getVariables(s, memberVariables)) {
			for (auto& variable : variables.value()) {
				using IRProxy::AccessModifier;
				switch (variable.m_modifier) {
					case AccessModifier::Public:
						s.m_public.m_variables.push_back(variable.m_variable);
						break;
					case AccessModifier::Private:
						s.m_private.m_variables.push_back(variable.m_variable);
						break;
					case AccessModifier::Protected:
						s.m_protected.m_variables.push_back(
						    variable.m_variable);
						break;
				}
			}
		}
	}
}

void buildStructs(std::vector<IRProxy::Struct>& structs,
                  IR::Namespace& globalNamespace) {
	for (auto& s : structs) {
		addStruct(s, globalNamespace);
	}
}

}    // namespace Builders
