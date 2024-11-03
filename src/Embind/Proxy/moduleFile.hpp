#pragma once

#include "Embind/Proxy/module.hpp"

#include <filesystem>
#include <string>
#include <vector>

namespace Embind::Proxy {

class ModuleFile {
  public:
  explicit ModuleFile(Module const& rootModule, std::string const& libraryName);

  /**
	* Add a module to the file.
	* NOTE: The order these are added is the order they will be put in the file
	*/
  void addModule(Module const& m);

  // Copies the type info into this classes
  void setTypeInfo(Embind::Proxy::TypeInfo const& info);

  std::filesystem::path getCppFilepath() const;

  std::filesystem::path getPreJSFilepath() const;

  std::string getEmbind() const;

  std::string getPreJS() const;

  private:
  std::string m_rootModuleName;
  std::string m_libraryName;

  // Will be put one after each other
  // The modules are responsible for which of them are submodules
  std::vector<Module> m_modules;

  // Contains global information about types in the module
  // E.g. if there is a type that forces some pybind include
  Embind::Proxy::TypeInfo m_typeInfo;
};
}    // namespace Embind::Proxy
