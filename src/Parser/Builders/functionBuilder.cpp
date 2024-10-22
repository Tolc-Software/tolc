#include "Builders/functionBuilder.hpp"
#include "Builders/commonBuilder.hpp"
#include "Builders/structBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Helpers/getStructData.hpp"
#include "Helpers/walkIRStructure.hpp"
#include "Visitor/Helpers/getDocumentation.hpp"

#include <IR/ir.hpp>
#include <clang/AST/ASTContext.h>
#include <clang/AST/Decl.h>
#include <clang/AST/DeclCXX.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>
#include <clang/Basic/Specifiers.h>
#include <llvm/Support/Casting.h>

#include <algorithm>
#include <deque>
#include <functional>
#include <optional>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace {
struct FunctionData {
  std::optional<IRProxy::AccessModifier> m_modifier;
  std::optional<IR::Operator> m_operator;
  bool m_isConstructor;
  bool m_isDestructor;
};

void addFunctionToVariant(FunctionData data,
                          std::variant<IR::Namespace*, IR::Struct*> const& v,
                          IR::Function f) {
  if (auto ns = std::get_if<IR::Namespace*>(&v)) {
    auto& functions = (*ns)->m_functions;
    functions.push_back(f);
  } else if (auto irStruct = std::get_if<IR::Struct*>(&v)) {
    // Add to the correct container in the struct
    if (auto structData = Helpers::getStructDataBasedOnAccess(
            **irStruct, data.m_modifier.value())) {
      if (data.m_isConstructor) {
        auto& functions = structData->m_constructors;
        functions.push_back(f);
      } else if (data.m_isDestructor) {
        auto& functions = structData->m_destructors;
        functions.push_back(f);
      } else if (data.m_operator) {
        auto& functions = structData->m_operators;
        functions.push_back({data.m_operator.value(), f});
      } else {
        auto& functions = structData->m_functions;
        functions.push_back(f);
      }
    }
  }
}

IR::Function createFunction(std::string_view name,
                            IRProxy::Function const& proxyF) {
  IR::Function f;
  f.m_name = name;
  f.m_id = proxyF.m_id;

  // Representation is the fully qualified name
  f.m_representation = proxyF.m_fullyQualifiedName;
  f.m_documentation = proxyF.m_documentation;
  f.m_returnType = proxyF.m_returnType;
  f.m_arguments = proxyF.m_arguments;
  f.m_templateArguments = proxyF.m_templateArguments;
  f.m_isStatic = proxyF.m_isStatic;
  f.m_polymorphic = proxyF.m_polymorphic;

  return f;
}

void addFunction(IRProxy::Function const& f, IR::Namespace& globalNamespace) {
  // Take out the already created path
  auto path = f.m_path;
  auto [name, t] = path.back();
  path.pop_back();

  // Find the parent
  auto parentOfNewFunction =
      Helpers::walkPathThroughStructure(path, globalNamespace);

  FunctionData data;
  data.m_isConstructor = f.m_isConstructor;
  data.m_isDestructor = f.m_isDestructor;
  data.m_modifier = f.m_modifier;
  data.m_operator = f.m_operator;

  // Create and add the function
  addFunctionToVariant(data, parentOfNewFunction, createFunction(name, f));
}

/**
* Tries to parse out a known operator from op
* Assumes op is whatever comes AFTER "operator" in the function name
* Ex:
*   operator+
*     getOperator("+") -> IR::Operator::Addition
*/
std::optional<IR::Operator> getOperator(std::string const& op) {
  using IR::Operator;
  if (op == "+") {
    return Operator::Addition;
  } else if (op == "-") {
    return Operator::Subtraction;
  } else if (op == "*") {
    return Operator::Multiplication;
  } else if (op == "/") {
    return Operator::Division;
  } else if (op == "%") {
    return Operator::Modulus;
  } else if (op == "=") {
    return Operator::Assignment;
  } else if (op == "+=") {
    return Operator::AddEqual;
  } else if (op == "-=") {
    return Operator::SubEqual;
  } else if (op == "*=") {
    return Operator::MulEqual;
  } else if (op == "/=") {
    return Operator::DivEqual;
  } else if (op == "%=") {
    return Operator::ModEqual;
  } else if (op == "<<") {
    return Operator::LeftShift;
  } else if (op == ">>") {
    return Operator::RightShift;
  } else if (op == "++") {
    return Operator::Increment;
  } else if (op == "--") {
    return Operator::Decrement;
  } else if (op == "==") {
    return Operator::Equal;
  } else if (op == "!=") {
    return Operator::NotEqual;
  } else if (op == ">") {
    return Operator::GreaterThan;
  } else if (op == ">=") {
    return Operator::GreaterThanOrEqualTo;
  } else if (op == "<") {
    return Operator::LessThan;
  } else if (op == "<=") {
    return Operator::LessThanOrEqualTo;
  } else if (op.front() == '[') {
    return Operator::Subscript;
  } else if (op.front() == '(') {
    return Operator::Call;
  }

  return std::nullopt;
}

IR::Polymorphic getPolymorphic(clang::FunctionDecl* f) {
  using IR::Polymorphic;
  if (f->isPureVirtual()) {
    return Polymorphic::PureVirtual;
  } else if (f->isVirtualAsWritten()) {
    return Polymorphic::Virtual;
  }
  return Polymorphic::NA;
}
}    // namespace

namespace Builders {

std::pair<FunctionError, std::optional<IRProxy::Function>>
buildFunction(clang::FunctionDecl* functionDecl,
              std::function<clang::QualType(clang::QualType)>
                  getPotentiallyTemplatedType) {
  IRProxy::Function parsedFunc;
  parsedFunc.m_fullyQualifiedName = functionDecl->getQualifiedNameAsString();

  parsedFunc.m_documentation = Visitor::Helpers::getDocumentation(functionDecl);
  parsedFunc.m_path =
      Builders::buildStructure(functionDecl, IRProxy::Structure::Function);

  parsedFunc.m_polymorphic = getPolymorphic(functionDecl);

  parsedFunc.m_operator = std::nullopt;
  auto& simpleName = parsedFunc.m_path.back().first;
  std::string_view constexpr op = "operator";
  if (simpleName.starts_with(op) && simpleName.size() > op.size()) {
    auto opType = simpleName.substr(op.size());
    if (auto maybeOperator = getOperator(opType)) {
      parsedFunc.m_operator = maybeOperator.value();
    } else {
      return {FunctionError::UnsupportedOperator, std::nullopt};
    }
  }

  // This is passed so that while extracting text from types it is exactly what the user wrote
  auto policy =
      clang::PrintingPolicy(functionDecl->getASTContext().getLangOpts());

  if (auto returnType = Builders::buildType(
          getPotentiallyTemplatedType(functionDecl->getReturnType()), policy)) {
    parsedFunc.m_returnType = returnType.value();
  } else {
    return {FunctionError::ReturnType, std::nullopt};
  }

  for (auto& p : functionDecl->parameters()) {
    if (auto argType = Builders::buildType(
            getPotentiallyTemplatedType(p->getType()), policy)) {
      IR::Argument arg;
      arg.m_name = p->getName();
      arg.m_type = argType.value();
      parsedFunc.m_arguments.push_back(arg);
    } else {
      return {FunctionError::ArgumentType, std::nullopt};
    }
  }

  // Check for access modifiers (public, private, ...)
  parsedFunc.m_modifier =
      Builders::convertToIRAccess(functionDecl->getAccess());

  parsedFunc.m_isStatic = functionDecl->isStatic();

  parsedFunc.m_isConstructor =
      llvm::isa<clang::CXXConstructorDecl>(functionDecl);
  parsedFunc.m_isDestructor = llvm::isa<clang::CXXDestructorDecl>(functionDecl);

  return {FunctionError::Ok, parsedFunc};
}

void buildFunctions(const std::vector<IRProxy::Function>& functions,
                    IR::Namespace& globalNamespace) {
  for (auto const& f : functions) {
    addFunction(f, globalNamespace);
  }
}

}    // namespace Builders
