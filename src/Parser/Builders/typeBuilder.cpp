#include "Builders/typeBuilder.hpp"
#include "Helpers/Type/matchers.hpp"
#include "Helpers/Type/utilities.hpp"
#include "Helpers/Utils/string.hpp"
#include "IRProxy/IRData.hpp"

#include <clang/AST/Decl.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/AST/PrettyPrinter.h>
#include <clang/AST/Type.h>
#include <llvm/Support/Casting.h>

#include <optional>
#include <queue>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace Builders {

struct Integral {
  // Might be 3 from std::array<int, 3>
  std::string i;
};

/**
* Returns a vector of QualType's which corresponds to the inputs template arguments list
* E.g.
*   std::vector<int, std::allocator<int>> -> {int, std::allocator<int>}
*/
std::vector<std::variant<clang::QualType, Integral>>
getTemplateArgs(clang::CXXRecordDecl const* classDecl) {
  std::vector<std::variant<clang::QualType, Integral>> templateArgs;

  if (auto templatedClass =
          llvm::dyn_cast<clang::ClassTemplateSpecializationDecl>(classDecl)) {
    for (auto const& arg : templatedClass->getTemplateArgs().asArray()) {
      switch (arg.getKind()) {
        case clang::TemplateArgument::Type:
          templateArgs.push_back(arg.getAsType());
          break;
        case clang::TemplateArgument::StructuralValue:
        case clang::TemplateArgument::Declaration:
          templateArgs.push_back(arg.getAsDecl()->getType());
          break;
        case clang::TemplateArgument::Integral: {
          // Extract 3 from std::array<int, 3>
          // The 10 means it should be represented as base 10
          templateArgs.push_back(
              Integral {std::to_string(arg.getAsIntegral().getExtValue())});
          break;
        }
        case clang::TemplateArgument::Null:
        case clang::TemplateArgument::NullPtr:
        case clang::TemplateArgument::Template:
          /// The template argument is a pack expansion of a template name that was
          /// provided for a template template parameter.
        case clang::TemplateArgument::TemplateExpansion:
          /// The template argument is an expression, and we've not resolved it to one
          /// of the other forms yet, either because it's dependent or because we're
          /// representing a non-canonical template argument (for instance, in a
          /// TemplateSpecializationType).
        case clang::TemplateArgument::Expression:
          break;
          /// The template argument is actually a parameter pack. Arguments are stored
          /// in the Args struct.
        case clang::TemplateArgument::Pack:
          // This is a tuple like structure
          for (auto p : arg.getPackAsArray()) {
            // Do not support nested tuples
            if (p.getKind() == clang::TemplateArgument::Type) {
              templateArgs.push_back(p.getAsType());
            }
          }
          break;
      }
    }
  }

  return templateArgs;
}

std::vector<std::variant<clang::QualType, Integral>>
getTemplateArgs(clang::QualType type) {
  if (auto classType = type->getAsCXXRecordDecl()) {
    return getTemplateArgs(classType);
  }
  return {};
}

/**
* True if it is a template specialization. E.g. std::vector<int>
*/
bool isTemplateSpecialization(clang::QualType type) {
  if (auto classDecl = type->getAsCXXRecordDecl()) {
    return llvm::isa<clang::ClassTemplateSpecializationDecl>(classDecl);
  }
  return false;
}

/**
* True if it is a template specialization. E.g. std::vector<int>
*/
bool isTemplateSpecialization(IR::Type const& type) {
  // TODO: This should be true for user defined templates aswell maybe?
  return std::get_if<IR::Type::Container>(&type.m_type);
}

/**
* Builds a shallow type. This does not include template arguments.
* Note that this is the complete type if the input is 'simple' as say 'int const*'
*/
std::optional<IR::Type> buildOneLevelIRType(clang::QualType type,
                                            clang::PrintingPolicy policy) {
  // What the user wrote
  auto representation = type.getAsString(policy);

  // Remove using aliases
  type = type.getCanonicalType();

  // Remove pointers
  auto numPointers = Helpers::Type::getNumberOfPointers(type);
  type = Helpers::Type::getTypeWithPointersRemoved(type);

  // Remove references
  auto hasReference = type.getTypePtr()->isReferenceType();
  type = type.getNonReferenceType();

  // Remove const
  // TODO: Handle unsupported qualifiers
  bool hasConst = Helpers::Type::isConst(type);
  type = type.getUnqualifiedType();

  // NOTE: Here we are not including the policy since we want the actual type,
  //       and not what the user wrote
  if (auto maybeIrType = Helpers::Type::getIRType(type.getAsString())) {
    auto irType = maybeIrType.value();

    // User defined types has a string type as {const struct UserDefined&}
    // But since we are not interested in the "struct " part, we remove it if it exists
    for (auto classPrefix : {"struct ", "class ", "enum "}) {
      representation =
          Helpers::Utils::removeSubString(representation, classPrefix);
    }
    irType.m_representation = representation;
    irType.m_numPointers = numPointers;
    irType.m_isConst = hasConst;
    irType.m_isReference = hasReference;
    return irType;
  }

  return {};
}

/**
* Used to link an IR type with its template arguments
*/
struct ProxyType {
  IR::Type* m_type;
  std::vector<std::pair<IR::Type, std::variant<clang::QualType, Integral>>>
      m_templateArgs;
};

IR::Type buildIRTypeFromIntegral(Integral const& integral) {
  IR::Type type;
  // The actual number
  type.m_representation = integral.i;

  // An integral is just a number
  type.m_numPointers = 0;
  type.m_isReference = false;
  type.m_isConst = false;
  type.m_type = IR::Type::Integral();

  return type;
}

/**
* Builds a ProxyType that contains template arguments of irType
*/
ProxyType buildProxyType(IR::Type& irType,
                         std::variant<clang::QualType, Integral> typeOrIntegral,
                         clang::PrintingPolicy policy) {
  ProxyType p;
  p.m_type = &irType;
  // Check if we need to go even further
  if (auto type = std::get_if<clang::QualType>(&typeOrIntegral)) {
    auto unQualifiedType = type->getUnqualifiedType();
    unQualifiedType = unQualifiedType.getNonReferenceType();
    unQualifiedType =
        Helpers::Type::getTypeWithPointersRemoved(unQualifiedType);
    if (isTemplateSpecialization(irType) ||
        isTemplateSpecialization(unQualifiedType)) {
      for (auto templateArg : getTemplateArgs(unQualifiedType)) {
        if (auto qualType = std::get_if<clang::QualType>(&templateArg)) {
          if (auto irTemplateArg = buildOneLevelIRType(*qualType, policy)) {
            p.m_templateArgs.emplace_back(
                std::make_pair(irTemplateArg.value(), templateArg));
          }
        } else if (auto integral = std::get_if<Integral>(&templateArg)) {
          p.m_templateArgs.emplace_back(
              std::make_pair(buildIRTypeFromIntegral(*integral), templateArg));
        }
      }
    }
  }
  return p;
}

/**
* Try to get the container type from type, otherwise nullptr
*/
IR::Type::Container* getContainer(IR::Type* type) {
  return std::get_if<IR::Type::Container>(&type->m_type);
}

std::optional<IR::Type> buildType(clang::QualType type,
                                  clang::PrintingPolicy policy) {
  std::optional<IR::Type> topLevelType = std::nullopt;
  std::queue<ProxyType> typesToProcess;
  if (auto irType = buildOneLevelIRType(type, policy)) {
    topLevelType = irType.value();
    typesToProcess.push(buildProxyType(topLevelType.value(), type, policy));
  }

  // Process template arguments
  while (!typesToProcess.empty()) {
    auto& currentType = typesToProcess.front();
    if (auto container = getContainer(currentType.m_type)) {
      // Is needed to avoid pointer to reallocated vector entry
      container->m_containedTypes.reserve(currentType.m_templateArgs.size());

      for (auto const& [irTemplateArg, clangTemplateArg] :
           currentType.m_templateArgs) {
        container->m_containedTypes.push_back(irTemplateArg);

        typesToProcess.push(buildProxyType(
            container->m_containedTypes.back(), clangTemplateArg, policy));
      }
    }

    // NOTE: This has to be at the bottom for currentType to survive this long
    typesToProcess.pop();
  }

  return topLevelType;
}

}    // namespace Builders
