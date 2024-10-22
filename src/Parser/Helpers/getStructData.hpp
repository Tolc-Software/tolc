#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>
#include <clang/AST/DeclBase.h>

namespace Helpers {

/**
* Find the data prefixed with accessModifier (i.e. public/private/protected)
* This contains functions etc.
* Returns nullptr if not found (should not happen)
*/
IR::StructData* getStructDataBasedOnAccess(IR::Struct& s,
                                           IRProxy::AccessModifier access);

IRProxy::StructData* getStructDataBasedOnAccess(IRProxy::Struct& s,
                                                clang::AccessSpecifier access);
}
