#pragma once

#include "IRProxy/IRData.hpp"
#include <IR/ir.hpp>

namespace Visitor::Helpers {
void addIdToFunction(IRProxy::Function& f, IRProxy::IRData& data);
void addIdToClass(IRProxy::Struct& s, IRProxy::IRData& data);
void addIdToVariable(IR::Variable& v, IRProxy::IRData& data);
void addIdToEnum(IRProxy::Enum& e, IRProxy::IRData& data);
}    // namespace Visitor::Helpers
