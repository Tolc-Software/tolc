#include "Swift/Builders/moduleFileBuilder.hpp"
#include "Objc/Proxy/class.hpp"
#include "Objc/Proxy/structure.hpp"
#include "Swift/Builders/classBuilder.hpp"
#include "Swift/Builders/functionBuilder.hpp"
#include "Swift/Builders/moduleBuilder.hpp"
#include "Swift/Proxy/moduleFile.hpp"
#include "Swift/cache.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <set>
#include <string>

namespace Swift::Builders {

std::optional<Swift::Proxy::ModuleFile>
buildModuleFile(std::vector<Objc::Proxy::Structure const*> const& structures,
                std::string const& moduleName) {
	Swift::Proxy::ModuleFile rootFile(moduleName);

	Swift::Cache cache;
	cache.m_moduleName = moduleName;
	cache.m_definedSymbols = {moduleName};

	for (auto const* structure : structures) {
		using Kind = Objc::Proxy::Structure::Kind;
		switch (structure->m_kind) {
			case Kind::Class: {
				rootFile.addClass(buildClass(
				    static_cast<Objc::Proxy::Class const&>(*structure),
				    cache.m_moduleName));
				break;
			}
			case Kind::Namespace: {
				for (auto const& module : buildModules(
				         static_cast<Objc::Proxy::Class const&>(*structure),
				         cache)) {
					rootFile.addModule(module);
				}
				break;
			}
			case Kind::Function: {
				rootFile.addFunction(buildFunction(
				    static_cast<Objc::Proxy::Function const&>(*structure),
				    cache.m_moduleName));
				break;
			}
			case Kind::Enum: {
				break;
			}
			case Kind::Attribute: {
				break;
			}
			break;
		}
	}

	return rootFile;
}
}    // namespace Swift::Builders
