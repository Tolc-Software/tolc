#include "Embind/Proxy/function.hpp"
#include "Embind/Helpers/string.hpp"

#include <fmt/format.h>

#include <algorithm>
#include <iterator>
#include <string>
#include <vector>

namespace Embind::Proxy {

namespace {

std::string getEmbindSelect(std::string const& signature,
                            std::string const& functionName) {
  return fmt::format("select_overload<{}>(&{})", signature, functionName);
}

std::string getFunctionPrefix(bool isClassFunction, bool isStatic) {
  // Defined inside a class
  // => No need for namespaces
  if (isClassFunction) {
    // Static functions are prefixed "class_"
    return isStatic ? "class_" : "";
  }
  return "\tem::";
}

}    // namespace

std::string Function::getEmbind(std::string const& namePrefix) const {
  std::string f;
  if (m_isConstructor) {
    // Results in
    // constructor<std::string, int, double>)
    f = fmt::format("constructor<{}>()\n", getArgumentTypes());
  } else {
    // Results in
    // class_function("myFunction", select_overload<void()>(&MyNS::myFunction))
    f = fmt::format(
        R"({functionPrefix}function("{name}", {qualifiedName}{allowPointers}{virtual}){ending})",
        fmt::arg("functionPrefix",
                 getFunctionPrefix(m_isClassFunction, m_isStatic)),
        fmt::arg("name", createName(namePrefix)),
        fmt::arg("allowPointers",
                 m_allowPointers ? ", em::allow_raw_pointers()" : ""),
        fmt::arg("virtual", m_polymorphic),
        fmt::arg("ending", m_isClassFunction ? "\n" : ";\n"),
        fmt::arg("qualifiedName",
                 m_isOverloaded ?
                     getEmbindSelect(getSignature(), m_fullyQualifiedName) :
                     '&' + m_fullyQualifiedName));
  }

  return f;
}

std::string Function::getPreJS(std::string const& namePrefix,
                               std::vector<std::string>& namesToDelete) const {
  // Will no longer need the old name
  namesToDelete.push_back(createName(namePrefix));

  // Renaming the function
  // Expects to be injected where necessary
  return fmt::format("\t\t\t{baseName}: Module['{globalName}'],\n",
                     fmt::arg("baseName", m_name),
                     fmt::arg("globalName", namesToDelete.back()));
}

std::string Function::createOverloadedName() const {
  std::vector<std::string> types;
  for (auto const& [argType, _argName] : m_arguments) {
    types.push_back(argType);
  }
  auto joinedArgs = fmt::format("{}", fmt::join(types, "_"));

  return joinedArgs.empty() ? m_name : m_name + '_' + joinedArgs;
}

std::string Function::getArgumentNames() const {
  // Get the typenames of the arguments
  std::vector<std::string> names;
  std::transform(m_arguments.begin(),
                 m_arguments.end(),
                 std::back_inserter(names),
                 [=](auto const& argument) { return argument.name; });
  return fmt::format("{}", fmt::join(names, ", "));
}

std::string Function::getArgumentTypes(bool withArgumentNames) const {
  // Get the typenames of the arguments
  std::vector<std::string> typeNames;
  std::transform(m_arguments.begin(),
                 m_arguments.end(),
                 std::back_inserter(typeNames),
                 [=](auto const& argument) {
                   return withArgumentNames ?
                              argument.typeName + " " + argument.name :
                              argument.typeName;
                 });
  return fmt::format("{}", fmt::join(typeNames, ", "));
}

std::string Function::getSignature() const {
  return fmt::format(
      R"(({returnType}({namespace}*)({arguments})))",
      fmt::arg("returnType", m_returnType),
      fmt::arg("namespace",
               Embind::Helpers::removeSubString(m_fullyQualifiedName, m_name)),
      fmt::arg("arguments", getArgumentTypes()));
}

std::string Function::createName(std::string const& namePrefix) const {
  return fmt::format(
      R"({namePrefix}{name})",
      fmt::arg("namePrefix", namePrefix),
      fmt::arg("name", m_isOverloaded ? createOverloadedName() : m_name));
}

Function::Function(std::string const& name,
                   std::string const& fullyQualifiedName)
  : m_name(name), m_fullyQualifiedName(fullyQualifiedName),
    m_returnType("void"), m_arguments({}), m_polymorphic(""),
    m_isConstructor(false), m_isOverloaded(false), m_isStatic(false),
    m_isClassFunction(false), m_allowPointers(false) {}

void Function::addArgument(std::string const& typeName,
                           std::string const& name) {
  m_arguments.push_back({typeName, name});
};

void Function::setReturnType(std::string const& typeName) {
  m_returnType = typeName;
}

void Function::setAsClassFunction() {
  m_isClassFunction = true;
};

void Function::setAsConstructor() {
  m_isConstructor = true;
};

void Function::setAsStatic() {
  m_isStatic = true;
};

void Function::setAsOverloaded() {
  m_isOverloaded = true;
};

std::string Function::getReturnType() const {
  return m_returnType;
}

std::string Function::getName() const {
  return m_name;
}

void Function::setAsPureVirtual() {
  m_polymorphic = ", em::pure_virtual()";
}

void Function::allowPointers() {
  m_allowPointers = true;
}

void Function::setAsVirtual(std::string const& base) {
  m_polymorphic = fmt::format(
      R"_tolc_delimiter(, optional_override([]({base}& self, {arguments}) {{
			return self.{base}::{functionName}({argumentNames});
		}}))_tolc_delimiter",
      fmt::arg("base", base),
      fmt::arg("arguments", getArgumentTypes(true)),
      fmt::arg("functionName", m_name),
      fmt::arg("argumentNames", getArgumentNames()));
}

}    // namespace Embind::Proxy
