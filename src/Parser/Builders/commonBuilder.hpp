#pragma once

#include "Helpers/Utils/split.hpp"
#include "IRProxy/IRData.hpp"
#include <clang/AST/Type.h>
#include <deque>
#include <optional>
#include <string>
#include <string_view>

namespace Builders {

/**
  * Convert the clang::AccessSpecifier to IRProxy::AccessModifier and add them to the structure
  */
std::optional<IRProxy::AccessModifier>
convertToIRAccess(clang::AccessSpecifier access);

/**
* @brief: Create a deque combining the type and the names of each DeclContext
*         NOTE: Assumes the last in the names deque is the name of the first parent
*
* @param: clang::DeclContext const* parent,
*         std::deque<std::string> names
*
* @return: std::deque<std::pair<std::string, IRProxy::Structure>>
*/
std::deque<std::pair<std::string, IRProxy::Structure>>
buildParentStructure(clang::DeclContext const* parent,
                     std::deque<std::string> names);

/**
* @brief: Create a deque combining the type and the names of each DeclContext
*
* @param: ClangDecl const* clangDecl,
*         IRProxy::Structure baseStructure - The type of the ClangDecl
*
* @return: std::deque<std::pair<std::string, IRProxy::Structure>>
*/
template <typename ClangDecl>
std::deque<std::pair<std::string, IRProxy::Structure>>
buildStructure(ClangDecl const* clangDecl, IRProxy::Structure baseStructure) {
	auto splitNames =
	    Helpers::Utils::split(clangDecl->getQualifiedNameAsString(), "::");

	auto nameOfDecl = splitNames.back();
	splitNames.pop_back();
	// Build from where we don't know the declstructure
	auto structure =
	    Builders::buildParentStructure(clangDecl->getParent(), splitNames);
	// Push the base back in
	structure.push_back({nameOfDecl, baseStructure});
	return structure;
}

}    // namespace Builders
