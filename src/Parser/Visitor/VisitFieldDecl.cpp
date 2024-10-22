#include "Builders/commonBuilder.hpp"
#include "Builders/fieldBuilder.hpp"
#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/utilities.hpp"
#include "IRProxy/IRData.hpp"
#include "Visitor/Helpers/addId.hpp"
#include "Visitor/ParserVisitor.hpp"

#include <clang/AST/Decl.h>
#include <spdlog/spdlog.h>

namespace Visitor {
bool ParserVisitor::VisitFieldDecl(clang::FieldDecl* fieldDecl) {
  if (isInSystemHeader(fieldDecl) || isPureTemplate(fieldDecl)) {
    // Continue the AST search
    return true;
  }
  spdlog::debug(R"(Parsing member variable: "{}")",
                fieldDecl->getQualifiedNameAsString());

  if (auto maybeField = Builders::buildField(fieldDecl)) {
    auto& [access, variable] = maybeField.value();
    IRProxy::MemberVariable proxyVariable;
    Helpers::addIdToVariable(variable, m_irData);
    proxyVariable.m_variable = variable;
    proxyVariable.m_modifier = access;

    // Add so it can be put into the IR AST later
    m_irData
        .m_memberVariables[fieldDecl->getParent()->getQualifiedNameAsString()]
        .push_back(proxyVariable);

  } else {
    spdlog::error(R"(Failed to parse member variable "{}")",
                  fieldDecl->getQualifiedNameAsString());
    m_parsedSuccessfully = false;
    // Stop parsing
    return false;
  }

  // Continue the AST search
  return true;
}
}    // namespace Visitor
