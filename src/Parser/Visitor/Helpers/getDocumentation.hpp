#pragma once

#include <clang/AST/Decl.h>
#include <string>

namespace Visitor::Helpers {

/**
* Extracts the formatted documentation string from decl
*/
std::string getDocumentation(clang::Decl* decl);
}
