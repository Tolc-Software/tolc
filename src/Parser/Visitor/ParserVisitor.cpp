#include "Visitor/ParserVisitor.hpp"
#include "Builders/enumBuilder.hpp"
#include "Builders/functionBuilder.hpp"
#include "Builders/namespaceBuilder.hpp"
#include "Builders/structBuilder.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/Type.h>
#include <fmt/format.h>
#include <llvm/Support/Casting.h>
#include <spdlog/spdlog.h>

#include <set>
#include <string>

namespace Visitor {

ParserVisitor::ParserVisitor(clang::ASTContext* context,
                             IR::Namespace& parsedNamespaces,
                             bool& parsedSuccessfully)
  : m_context(context), m_parsedNamespaces(parsedNamespaces),
    m_parsedSuccessfully(parsedSuccessfully) {}

ParserVisitor::~ParserVisitor() {
  if (m_parsedSuccessfully) {
    // Build empty namespaces and their relations
    m_parsedNamespaces =
        Builders::buildNamespaceStructure(m_irData.m_namespaces);

    // Add global variables to namespaces
    Builders::addGlobalVariables(m_irData.m_globalVariables,
                                 m_parsedNamespaces);

    // Add the collected member variables to the structs
    Builders::addMemberVariables(m_irData.m_structs,
                                 m_irData.m_memberVariables);

    // Add the structs/classes to the namespaces
    Builders::buildStructs(m_irData.m_structs, m_parsedNamespaces);

    // Add the functions to the namespaces
    Builders::buildFunctions(m_irData.m_functions, m_parsedNamespaces);

    // Add the enums to the namespaces
    Builders::buildEnums(m_irData.m_enums, m_parsedNamespaces);
  }
}

bool ParserVisitor::shouldVisitTemplateInstantiations() const {
  return true;
}

bool ParserVisitor::isInSystemHeader(clang::Decl* decl) {
  return m_context->getSourceManager().isInSystemHeader(decl->getLocation());
}

/**
* Return a set of strings which corresponds to the inputs template arguments list
* E.g.
*   template<typename T, typename U>
*   class MyClass { ... }
*   ->
*   {"T", "U"}
*
* Returns an empty list for non-templates
*/
std::set<std::string> getTemplateArgs(clang::CXXRecordDecl const* classDecl) {
  std::set<std::string> templateArgs;
  if (auto templatedClass = classDecl->getDescribedClassTemplate()) {
    for (auto const& arg : templatedClass->getTemplateParameters()->asArray()) {
      templateArgs.insert(arg->getNameAsString());
    }
  }

  return templateArgs;
}

bool ParserVisitor::isPureTemplate(clang::FieldDecl* fieldDecl) {
  bool isTemplate = false;
  auto record = fieldDecl->getParent();
  // Check if parent class is a template that has the type as a template parameter
  // Ex:
  //   template <typename T>
  //   class MyClass {
  //     T myMember;
  //   };
  if (llvm::isa<clang::CXXRecordDecl>(record)) {
    if (auto templateArgs =
            getTemplateArgs(llvm::cast<clang::CXXRecordDecl>(record));
        templateArgs.find(fieldDecl->getType().getAsString()) !=
        templateArgs.end()) {
      isTemplate = true;
    }
  }
  return isTemplate;
}

bool ParserVisitor::isPureTemplate(clang::FunctionDecl* functionDecl) {
  bool isTemplate = false;

  // Is function directly a template?
  switch (functionDecl->getTemplatedKind()) {
    using clang::FunctionDecl;
    // Not templated.
    case (FunctionDecl::TemplatedKind::TK_NonTemplate):
    case (FunctionDecl::TemplatedKind::TK_DependentNonTemplate):
      isTemplate = false;
      break;
    // The pattern in a function template declaration.
    case (FunctionDecl::TemplatedKind::TK_FunctionTemplate):
      isTemplate = true;
      break;
    // A non-template function that is an instantiation or explicit
    // specialization of a member of a templated class.
    case (FunctionDecl::TemplatedKind::TK_MemberSpecialization):
      isTemplate = true;
      break;
    // An instantiation or explicit specialization of a function template.
    // Note: this might have been instantiated from a templated class if it
    // is a class-scope explicit specialization.
    case (FunctionDecl::TemplatedKind::TK_FunctionTemplateSpecialization):
      isTemplate = true;
      break;
    // A function template specialization that hasn't yet been resolved to a
    // particular specialized function template.
    case (FunctionDecl::TemplatedKind::
              TK_DependentFunctionTemplateSpecialization):
      isTemplate = true;
      break;
  }
  if (isTemplate) {
    return isTemplate;
  }

  // Is the function types part of a class that is templated
  // Ex:
  //   template <typename T>
  //   class MyClass {
  //     T f(T arg);
  //     int fun();
  //   };
  //
  //   Both f and fun are considered part of a class template
  if (llvm::isa<clang::CXXMethodDecl>(functionDecl)) {
    isTemplate = llvm::cast<clang::CXXMethodDecl>(functionDecl)
                     ->getParent()
                     ->getDescribedClassTemplate() != nullptr;
  }

  return isTemplate;
}

bool ParserVisitor::isPureTemplate(clang::CXXRecordDecl* classDecl) {
  return !getTemplateArgs(classDecl).empty();
}

bool ParserVisitor::isTemplateSpecialization(clang::CXXRecordDecl* classDecl) {
  return llvm::isa<clang::ClassTemplateSpecializationDecl>(classDecl);
}

}    // namespace Visitor
