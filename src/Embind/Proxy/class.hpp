#pragma once

#include "Embind/Proxy/enum.hpp"
#include "Embind/Proxy/function.hpp"

#include <optional>
#include <string>
#include <vector>

namespace Embind::Proxy {

class Class {
  public:
  explicit Class(std::string const& name,
                 std::string const& fullyQualifiedName);

  void addEnum(Enum const& e);

  void addFunction(Function const& function);

  void addConstructor(Function const& constructor);

  void addMemberVariable(std::string const& variableName,
                         std::string const& getter,
                         std::optional<std::string> setter,
                         bool isStatic);

  std::string const& getName() const;

  // Will be managed by a std::shared_ptr on the python side
  // instead of the default std::unique_ptr
  void setAsManagedByShared();

  std::string getEmbind(std::string const& namePrefix = "") const;

  std::string getPreJS(std::string const& namePrefix,
                       std::vector<std::string>& namesToDelete) const;

  // Same as getPreJS but is assumed to be injected into the root of the Module
  std::string getGlobalPreJS(std::string const& namePrefix,
                             std::vector<std::string>& namesToDelete) const;

  void addTrampolineClass(std::string const& className,
                          std::string const& fullyQualifiedName);

  void setAsPure();

  void addInherited(std::string const& baseClass);

  private:
  struct MemberVariable {
    // User defined name of the member variable
    std::string m_name;
    // Function names to get and set the variable
    std::string m_getter;
    std::optional<std::string> m_setter;
    bool m_isStatic;
  };

  struct Trampoline {
    std::string m_name;
    std::string m_fullyQualifiedName;
  };

  // User defined name of the class
  std::string m_name;
  std::string m_fullyQualifiedName;

  std::vector<Trampoline> m_trampolineClasses;
  std::vector<std::string> m_inherited;

  std::vector<Function> m_constructors;
  std::vector<Function> m_functions;
  std::vector<MemberVariable> m_memberVariables;
  std::vector<Enum> m_enums;

  bool m_isManagedByShared;
  bool m_isPure;

  std::string createName(std::string const& namePrefix) const;
};
}    // namespace Embind::Proxy
