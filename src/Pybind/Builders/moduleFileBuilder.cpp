#include "Pybind/Builders/moduleFileBuilder.hpp"
#include "Pybind/Builders/moduleBuilder.hpp"
#include "Pybind/Helpers/combine.hpp"
#include "Pybind/Proxy/moduleFile.hpp"
#include "Pybind/Proxy/typeInfo.hpp"
#include <IR/ir.hpp>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct ModulePair {
	IR::Namespace const& m_namespace;
	Pybind::Proxy::Module m_module;
};
}    // namespace

namespace Pybind::Builders {

std::optional<Pybind::Proxy::ModuleFile>
buildModuleFile(IR::Namespace const& rootNamespace,
                std::string const& rootModuleName) {
	Pybind::Proxy::TypeInfo typeInfo;
	if (auto maybeRootModule = Pybind::Builders::buildModule(
	        rootNamespace, rootModuleName, typeInfo)) {
		auto rootModule = maybeRootModule.value();
		Pybind::Proxy::ModuleFile moduleFile(rootModule, rootModuleName);

		std::queue<ModulePair> namespaces;
		for (auto const& subNamespace : rootNamespace.m_namespaces) {
			if (auto m = Pybind::Builders::buildModule(
			        subNamespace, rootModuleName, typeInfo)) {
				namespaces.push({subNamespace, m.value()});
			} else {
				return std::nullopt;
			}
		}

		while (!namespaces.empty()) {
			auto const& [currentNamespace, currentModule] = namespaces.front();

			moduleFile.addModule(currentModule);

			// Go deeper into the nested namespaces
			for (auto const& subNamespace : currentNamespace.m_namespaces) {
				if (auto m = Pybind::Builders::buildModule(
				        subNamespace, rootModuleName, typeInfo)) {
					namespaces.push({subNamespace, m.value()});
				} else {
					return std::nullopt;
				}
			}

			// Need currentNamespace and currentModule to live this far
			namespaces.pop();
		}

		moduleFile.setTypeInfo(typeInfo);
		return moduleFile;
	}

	return std::nullopt;
}
}    // namespace Pybind::Builders
